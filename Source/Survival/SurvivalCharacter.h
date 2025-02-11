// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "WeaponPickupSystem/Character/CharacterTypes.h"
#include "WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "WeaponPickupSystem/CharacterBase/SurvivalCharacterBase.h"


#include "SurvivalCharacter.generated.h"


class UBoxComponent;
class USurvivalCharacterCombatComponent;
class ULockonComponent;
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
// class UInputMappingContext;
// class UInputAction;
// struct FInputActionValue;
class USphereComponent;
struct FGameplayAbilitySpecHandle;


DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*)

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASurvivalCharacter : public ASurvivalCharacterBase
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

	FOnTargetInteractedDelegate OnKickHitTarget;
	FOnTargetInteractedDelegate OnKickPulledFromTarget;
	
	/* GAS */
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	//--------

	UFUNCTION()
	void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* PickupSphere;
	
	UPROPERTY()
	ABasePickup* CurrentPickup; // TODO: TScriptInterface<IInteractionInterface>* CurrentPickup; Change with this!!! ??

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

// Movement Speed
	void OnMovementSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
	
// Health Component
	UFUNCTION()
	void InitializeResourceComponent();
	
	UPROPERTY()
	ASurvivalSystemHUD* HUD;

private:

//GAS-----
	// UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<UCharacterAbilitySystemComponent> CharacterAbilitySystemComponent;
	//
	// UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// UCharacterAttributeSet* CharacterAttributes;

	UPROPERTY(EditAnywhere, Category = "Custom values | Character Info")
	FGameplayTag CharacterTag; // Represents Character class (Tags like; Warrior, Mage, Archer...)
	
	void InitAbilityActorInfo();
	void InitClassDefaults();
	void BindResourceInitialization();
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
	
	UPROPERTY(VisibleAnywhere, Category = "Components | Character Lockon" )
	ULockonComponent* LockonComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components | Character Combat" )
	USurvivalCharacterCombatComponent* SurvivalCharacterCombatComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components | FootCollision" )
	UBoxComponent* FootCollision;
	
	FTimerHandle InitializeDelayTimerHandle;

	uint32 bIsCharacterInitialized:1;


public:

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
	// FORCEINLINE UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponent() const { return  CharacterAbilitySystemComponent; }
	// Current Pickup
	FORCEINLINE ABasePickup* GetCurrentPickup() const { return CurrentPickup; } // TODO: Pickup Component'a al!		
	FORCEINLINE void SetCurrentPickup(ABasePickup* NewPickup) { CurrentPickup = NewPickup; } // TODO: Pickup Component'a al!
	// Pickup Component
	FORCEINLINE UPickupComponent* GetPickupComponent() const { return PickupComponent; }
	// PickupSphere
	FORCEINLINE USphereComponent* GetPickupSphere() const { return PickupSphere; }
	// Inputs
	FORCEINLINE UGASEnhancedInputComponent* GetGASEnhancedInputComponent() const { return GASEnhancedInputComponent; }
	// Lockon Component
	FORCEINLINE ULockonComponent* GetLockonComponent() const { return LockonComponent; }
	// FootCollision
	FORCEINLINE UBoxComponent* GetFootCollision() const { return FootCollision; }
	
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	
	
	/* HUD */
	ASurvivalSystemHUD* GetSurvivalHUD() const;
	
};

