#include "FA_Interact.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "FrogCharacter/FrogCharacter.h"
#include "FrogGameplay/Core/Item.h"
#include "FrogGameplay/Core/Container.h"
#include "FrogGameplay/Core/ContainerComponent.h"
#include "Frog.h"

UFA_Interact::UFA_Interact()
{
    ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFA_Interact::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
       EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
       return;
    }

    const AFrogCharacter* Frog = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get());
    if (!IsValid(Frog))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    IInteractableInterface* ClosestInteractable = Frog->GetClosestInteractable();
    if (ClosestInteractable && HasAuthority(&ActivationInfo))
    {
        ClosestInteractable->StartInteract();
        PickupInteractable(Frog, Cast<AItem>(ClosestInteractable));
    }

    UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
    WaitInputReleaseTask->OnRelease.AddDynamic(this, &UFA_Interact::HandleInputReleased);
    WaitInputReleaseTask->ReadyForActivation();
}

void UFA_Interact::PickupInteractable(const AFrogCharacter* Frog, AItem* Item)
{
    if (!IsValid(Item)) return;

    AItem* OtherOffer = Item->GetOfferedInteractable();
    UContainerComponent* PlayerContainer = Frog->GetContainerComponent();
    if (!IsValid(OtherOffer) || !IsValid(PlayerContainer)) return;

    UContainerComponent* OtherContainerComp = nullptr;
    if (AContainer* OtherContainer = Cast<AContainer>(Item))
    {
        OtherContainerComp = OtherContainer->GetContainerComponent();
    }
    
    UContainerComponent* OtherOfferAsContainerComp = nullptr;
    if (AContainer* OtherOfferAsContainer = Cast<AContainer>(OtherOffer))
    {
        OtherOfferAsContainerComp = OtherOfferAsContainer->GetContainerComponent();
    }

    AItem* HeldInteractable = PlayerContainer->GetFirstItem();
    if (IsValid(OtherOfferAsContainerComp) && OtherOfferAsContainerComp->TryAddToInventory(HeldInteractable, PlayerContainer))
    {
        FLOG(TEXT("Trying interact as item..."));
        return;
    }        
    
    AContainer* HeldContainer = Cast<AContainer>(HeldInteractable);
    if (IsValid(HeldContainer))
    {
        FLOG(TEXT("Try interact as container..."));
        
        UContainerComponent* HeldContainerComp = HeldContainer->GetContainerComponent();
        if (IsValid(HeldContainerComp))
        {
            // Other Offer as Container <- Player Container Contents 
            if (IsValid(OtherOfferAsContainerComp) && OtherOfferAsContainerComp->TryAddContainerContentsToInventory(HeldContainerComp))
            {
                return;
            }
            
            // Player Container <- Other Offer as Container's Contents
            if (IsValid(OtherOfferAsContainerComp) && HeldContainerComp->TryAddContainerContentsToInventory(OtherOfferAsContainerComp))
            {
                return;
            }
            
            // Player Container <- Other Container's Offer (Item)
            if (IsValid(OtherContainerComp) && HeldContainerComp->TryAddToInventory(OtherOffer, OtherContainerComp))
            {
                return;
            }
            
            // Player container <- Item (not in a container)
            // Not currently supporting picking up loose items (on the ground)
            // if (HeldContainerComp->TryAddToInventory(HeldContainer))
            // {
            //     return;
            // }
        }
    }
    
    if (!IsValid(HeldInteractable))
    {
        FLOG(TEXT("Trying interact as player"));
        PlayerContainer->TryAddToInventory(OtherOffer, OtherContainerComp);
        return;
    }
}

void UFA_Interact::HandleInputReleased(float TimeHeld)
{
    const AFrogCharacter* Frog = Cast<AFrogCharacter>(GetAvatarActorFromActorInfo());
    if (IsValid(Frog))
    {
       IInteractableInterface* ClosestInteractable = Frog->GetClosestInteractable();
       if (ClosestInteractable && HasAuthority(&CurrentActivationInfo))
       {
          ClosestInteractable->StopInteract();
       }
    }
    
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}