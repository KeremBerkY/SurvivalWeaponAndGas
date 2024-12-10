// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "WeaponPickupSystem/Character/CharacterTypes.h"
#include "WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"


#include "SurvivalCharacter.generated.h"


class UPickupComponent;
class UGASEnhancedInputComponent;
class UResourceComponent;
class UCharacterGameplayAbility;
class UCharacterAbilitySystemComponent;
class UCharacterStateManager;
class UCharacterCameraComponent;
class UCharacterWeaponComponent;
class ASurvivalSystemHUD;
class UCharacterStateComponent;
class UCharacterAnimInstance;
class UWeaponInventory;
class AWeaponBase;
class ABasePickup;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class USphereComponent;
struct FGameplayAbilitySpecHandle;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

	// // TODO: TObjectPtr<UInputAction> yap
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	ASurvivalCharacter();
	
							/* Getters/Setters */
	// Camera
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCharacterCameraComponent* GetCharacterCameraComponent() const { return CharacterCameraComponent; }
	// Anim Instance
	FORCEINLINE UCharacterAnimInstance* GetCharacterAnimInstance() const { return CharacterAnimInstance; }
	// Character State
	FORCEINLINE ECharacterState GetCharacterState() const { return CurrentCharacterState; }
	FORCEINLINE void SetCharacterState(ECharacterState NewCharacterState) { CurrentCharacterState = NewCharacterState; }
	// Weapon Inventory Component
	FORCEINLINE UWeaponInventory* GetWeaponInventory() const { return WeaponInventoryComponent; }
	// Character State Component
	FORCEINLINE UCharacterStateComponent* GetCharacterStateComponent() const { return CharacterStateComponent; }
	// Character Weapon Component
	FORCEINLINE UCharacterWeaponComponent* GetCharacterWeaponComponent() const { return CharacterWeaponComponent; }
	// Ability System Component
	FORCEINLINE UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponent() const { return  CharacterAbilitySystemComponent; }
	// Current Pickup
	FORCEINLINE ABasePickup* GetCurrentPickup() const { return CurrentPickup; } // TODO: Pickup Component'a al!		
	FORCEINLINE void SetCurrentPickup(ABasePickup* NewPickup) { CurrentPickup = NewPickup; } // TODO: Pickup Component'a al!
	// Pickup Component
	FORCEINLINE UPickupComponent* GetPickupComponent() const { return PickupComponent; }
	// PickupSphere
	FORCEINLINE USphereComponent* GetPickupSphere() const { return PickupSphere; }
	
	/* HUD */
	ASurvivalSystemHUD* GetSurvivalHUD() const;
	

	/* GAS */
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	//--------
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* PickupSphere;
	
	UPROPERTY()
	ABasePickup* CurrentPickup; // TODO: TScriptInterface<IInteractionInterface>* CurrentPickup; Change with this!!! ??

	
protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;


// Health Component
	UFUNCTION()
	void InitializeResourceComponent();
	
	
	UPROPERTY()
	ASurvivalSystemHUD* HUD;

private:

//GAS-----
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterAbilitySystemComponent> CharacterAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UCharacterAttributeSet* CharacterAttributes;

	UPROPERTY(EditAnywhere, Category = "Custom values | Character Info")
	FGameplayTag CharacterTag; // Represents Character class (Tags like; Warrior, Mage, Archer...)
	
	void InitAbilityActorInfo();
	void InitClassDefaults();
	// void BindAbilityInput();
	
	
// -------

	void HandleInitializeDelay();

	ECharacterState CurrentCharacterState; 	// CharacterTypes.h
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "Components | Weapon Inventory", meta = (AllowPrivateAccess = "true"))
	UWeaponInventory* WeaponInventoryComponent;

	UPROPERTY(VisibleAnywhere, Category = "CharaterAnim")
	UCharacterAnimInstance* CharacterAnimInstance;

	UPROPERTY(VisibleAnywhere, Category = "Components | Character State")
	UCharacterStateComponent* CharacterStateComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components | Character Weapon")
	UCharacterWeaponComponent* CharacterWeaponComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components | Character Camera")
	UCharacterCameraComponent* CharacterCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components | Character Health")
	UResourceComponent* ResourceComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components | Character Health")
	UGASEnhancedInputComponent* GASEnhancedInputComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components | Pickups" )
	UPickupComponent* PickupComponent;
	
	FTimerHandle InitializeDelayTimerHandle;
	
};

