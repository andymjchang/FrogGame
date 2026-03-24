// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableInterface.h"

#include "GameplayTagContainer.h"
#include "ItemData.h"

void IInteractableInterface::StartInteract()
{
}

void IInteractableInterface::StopInteract()
{
}

void IInteractableInterface::StartHighlight(UMaterialInterface* InHighlightMaterial)
{
	UMeshComponent* Mesh = GetInteractableMesh();
	if (IsValid(Mesh) && IsValid(InHighlightMaterial) && Mesh->GetOverlayMaterial() == nullptr)
	{
		Mesh->SetOverlayMaterial(InHighlightMaterial);
	}
}

void IInteractableInterface::StopHighlight()
{
	UMeshComponent* Mesh = GetInteractableMesh();
	if (IsValid(Mesh))
	{
		Mesh->SetOverlayMaterial(nullptr);
	}
}


