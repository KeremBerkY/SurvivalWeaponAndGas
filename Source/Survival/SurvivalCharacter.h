// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
//// DENEME(1) /////
// #include "WeaponPickupSystem/PickupMaster.h"
// #include "WeaponMaster.generated.h" // command line yap derliyo o zaman ama cpp de SpawnWeapon tsubclassof parametresine hata veriyor.
// #include "WeaponPickupSystem/WeaponMaster.h" // bunu da sonradan ekledim

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "WeaponPickupSystem/Character/CharacterTypes.h"
#include "WeaponPickupSystem/Character/GAS/Abilities/AbilityInputs/GASAbilityInputID.h"

#include "AbilitySystemInterface.h"
#include "WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"


#include "SurvivalCharacter.generated.h"


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
	// /** MappingContext */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputMappingContext* DefaultMappingContext;
	// /** Jump Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* JumpAction;
	// /** Move Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* MoveAction;
	// /** Look Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* LookAction;
	// /** Interact Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* InteractAction;
	// /* Fire Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* FireAction;
	// /* Reload Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* ReloadAction;
	// /** Drop Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* DropAction;
	// /* Weapon Inventory Actions Weapon Swap */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* RaycastAction;
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* ProjectileAction;
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* MeleeAction;
	// /* ToggleFireMode Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* ToggleFireModeAction;
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction* SprintAction;
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
	FORCEINLINE ABasePickup* GetCurrentPickup() const { return CurrentPickup; }
	FORCEINLINE void SetCurrentPickup(ABasePickup* NewPickup) { CurrentPickup = NewPickup; }
	
	/* HUD */
	ASurvivalSystemHUD* GetSurvivalHUD() const;
	
	/* GOLD */
	UFUNCTION()
	void AddCoin(int32 CoinAdded) { TotalCoin += CoinAdded; }
	UFUNCTION()
	void PlayerTotalCoin() const;

	/* Pickups */
	UFUNCTION()
	void CheckPickupInterval(float DeltaTime);
	UFUNCTION()
	void CheckNearbyPickups();
	UFUNCTION()
	void UpdateCurrentPickup(ABasePickup* NewPickup);

	/* GAS */

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	//--------
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* PickupSphere;
	
	UPROPERTY()
	ABasePickup* CurrentPickup; // TODO: TScriptInterface<IInteractionInterface>* CurrentPickup; Change with this!!! ??

	
protected:
	
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

/* Input Methods */
	// void Move(const FInputActionValue& Value);
	// void Look(const FInputActionValue& Value);
	// void Interact();
	// void Drop();
	// void FirePressed();
	// void FireReleased();
	// void Reload();
	// void RaycastWeaponSwitch();
	// void ProjectileWeaponSwitch();
	// void MeleeWeaponSwitch();
	// void ToggleWeaponFireMode();
	// void StartSprinting();
	// void StopSprinting();
	
/* Time / Cooldown Methods */
	// bool CanSwitchWeapon() const;
	// void UpdateLastSwitchTime();
	//
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
	void BindAbilityInput();
	
	
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

	FTimerHandle InitializeDelayTimerHandle;

// TEST Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Switching", meta = (AllowPrivateAccess = "true"))
	float WeaponSwitchCooldown = 1.0f;
	UPROPERTY()
	float LastSwitchTime;

	bool bIsSprinting; // For Sprint state
	
	int32 TotalCoin;
	int32 Coin;
	
	// bool bIsFiring = false;
	
	
};

