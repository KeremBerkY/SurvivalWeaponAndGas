// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// UENUM(BlueprintType)
// enum class EInteractableType : uint8
// {
// 	Pickup UMETA(DisplayName = "Pickup"), // Gold (for now)
// 	Weapon UMETA(DisplayName = "Weapon"), // Raycast, Projectile & Melee Weapons
// 	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
// 	Device UMETA(DisplayName = "Device"), // door, gate, window
// 	Toggle UMETA(DisplayName = "Toggle"), // button, switch, activate some functionality
// 	Container UMETA(DisplayName = "Container"),
// };
//
// USTRUCT(BlueprintType)
// struct FInteractableData
// {
// 	GENERATED_USTRUCT_BODY()
//
// 	FInteractableData() : InteractableType(EInteractableType::Pickup), Name(FText::GetEmpty()),
// 			Action(FText::GetEmpty()), Quantity(0), InteractionDuration(0.f) {};
//
// 	
// 	UPROPERTY(EditAnywhere, Category = "Interactable Data")
// 	EInteractableType InteractableType;
// 	
// 	UPROPERTY(EditAnywhere, Category = "Interactable Data")
// 	FText Name;
// 	
// 	UPROPERTY(EditAnywhere, Category = "Interactable Data")
// 	FText Action;
// 	
// 	UPROPERTY(EditAnywhere, Category = "Interactable Data")
// 	int32 Quantity; // Used only for pickups
// 	
// 	UPROPERTY(EditAnywhere, Category = "Interactable Data")
// 	float InteractionDuration; // Used for things like valves, doors, etc. that require an interaction timer
// };


// This class does not need to be modified.
UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURVIVAL_API IInteractionInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Actor);
	
	// FInteractableData InteractableData;
};
