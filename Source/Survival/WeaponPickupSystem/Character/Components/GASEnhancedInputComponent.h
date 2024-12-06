// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/AbilityInputs/GASAbilityInputID.h"
#include "GASEnhancedInputComponent.generated.h"


class UAbilitySystemComponent;
class UInputMappingContext;
class UInputAction;

// touch gray, kent slim gray - beyaz filtreli kent verdiler. Memnun kalmadım

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UGASEnhancedInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGASEnhancedInputComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=PVDInput, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* AbilityMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RaycastAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ProjectileAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MeleeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ToggleFireModeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	virtual void BeginPlay() override;

	
	void HandleJumpActionPressed();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void HandleInteractActionPressed();
	void HandleFireActionPressed();
	void HandleReloadActionPressed();
	void HandleDropActionPressed();
	void HandleRaycastActionPressed();
	void HandleProjectileActionPressed();
	void HandleMeleeActionPressed();
	void HandleToggleFireModeActionPressed();
	void HandleSprintActionHold();

	void HandleJumpActionReleased();
	void HandleInteractActionReleased();
	void HandleFireActionReleased();
	void HandleReloadActionReleased();
	void HandleDropActionReleased();
	void HandleRaycastActionReleased();
	void HandleProjectileActionReleased();
	void HandleMeleeActionReleased();
	void HandleToggleFireModeActionReleased();
	void HandleSprintActionReleased();

private:
	void SendInputActionToASC(bool bIsPressed, const EGASAbilityInputID AbilityInputID) const;

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponentPtr;
};
