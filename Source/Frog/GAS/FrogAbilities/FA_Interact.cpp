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
        Interact(Frog, ClosestInteractable);
    }

    UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
    WaitInputReleaseTask->OnRelease.AddDynamic(this, &UFA_Interact::HandleInputReleased);
    WaitInputReleaseTask->ReadyForActivation();
}

void UFA_Interact::Interact(const AFrogCharacter* Frog, IInteractableInterface* Interactable)
{
    AItem* OtherItem = Cast<AItem>(Interactable);
    if (!IsValid(OtherItem)) return;

    AItem* OtherOffer = OtherItem->GetOfferedInteractable();
    UContainerComponent* PlayerContainer = Frog->GetContainerComponent();
    
    if (!IsValid(OtherOffer) || !IsValid(PlayerContainer))
    {
        return;
    }

    UContainerComponent* OtherContainerComp = nullptr;
    if (AContainer* OtherContainer = Cast<AContainer>(OtherItem))
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
            if (IsValid(OtherContainerComp) && HeldContainerComp->TryAddToInventory(OtherOffer, OtherContainerComp))
            {
                return;
            }
            
            if (IsValid(OtherOfferAsContainerComp) && OtherOfferAsContainerComp->TryAddContainerContentsToInventory(HeldContainer))
            {
                return;
            }
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