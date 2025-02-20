// Fill out your copyright notice in the Description page of Project Settings.


#include "GASEnhancedInputComponent.h"

#include "CharacterCameraComponent.h"
#include "CharacterWeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "LockonComponent.h"
#include "WeaponInventory.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"


UGASEnhancedInputComponent::UGASEnhancedInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UGASEnhancedInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetOwner()))
	{
		AbilitySystemComponentPtr = MakeWeakObjectPtr(PlayerCharacter->GetCharacterAbilitySystemComponent());
		if (const APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()))
		{
			ensure(AbilityMappingContext);

			if (const auto EnhancedInputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				EnhancedInputSubSystem->AddMappingContext(AbilityMappingContext, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleJumpActionPressed);
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleJumpActionReleased);

				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UGASEnhancedInputComponent::Move);
				EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UGASEnhancedInputComponent::Look);

				EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleInteractActionPressed);
				EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleInteractActionReleased);

				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleFireActionPressed);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleFireActionReleased);

				EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleReloadActionPressed);
				EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleReloadActionReleased);

				EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleDropActionPressed);
				EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleDropActionReleased);
				
				EnhancedInputComponent->BindAction(RaycastAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleRaycastActionPressed);
				EnhancedInputComponent->BindAction(RaycastAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleRaycastActionReleased);
				
				EnhancedInputComponent->BindAction(ProjectileAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleProjectileActionPressed);
				EnhancedInputComponent->BindAction(ProjectileAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleProjectileActionReleased);
				
				EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleMeleeActionPressed);
				EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleMeleeActionReleased);
				
				EnhancedInputComponent->BindAction(ToggleFireModeAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleToggleFireModeActionPressed);
				EnhancedInputComponent->BindAction(ToggleFireModeAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleToggleFireModeActionReleased);
				
				EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleSprintActionHold);
				EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleSprintActionReleased);
				
				EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleAimingButtonPressed);
				EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleAimingButtonReleased);
				
				EnhancedInputComponent->BindAction(LockonAction, ETriggerEvent::Started, this, &UGASEnhancedInputComponent::HandleLockonButtonPressed);
				EnhancedInputComponent->BindAction(LockonAction, ETriggerEvent::Completed, this, &UGASEnhancedInputComponent::HandleLockonButtonReleased);
			}
		}
	}
}


void UGASEnhancedInputComponent::HandleJumpActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Jumping Ability fired"));
	SendInputActionToASC(true, EGASAbilityInputID::Jump);
}

void UGASEnhancedInputComponent::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	AActor* Owner = GetOwner();
	if (!Owner) return;

	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(Owner))
	{
		const FRotator Rotation = PlayerCharacter->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		PlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		PlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UGASEnhancedInputComponent::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	AActor* Owner = GetOwner();
	if (!Owner) return;

	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(Owner))
	{
		PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
		PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
	}

}

void UGASEnhancedInputComponent::HandleInteractActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Interact Ability fired"));
	SendInputActionToASC(true, EGASAbilityInputID::Interact);
}

void UGASEnhancedInputComponent::HandleFireActionPressed()
{
	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetOwner()))
	{
		if (AWeaponBase* CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon())
		{
			if (CurrentWeapon->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_RaycastWeapons &&
				PlayerCharacter->GetCharacterCameraComponent()->IsAiming() || PlayerCharacter->GetLockonComponent()->IsLocked())
			{
				SendInputActionToASC(true, EGASAbilityInputID::Fire);
			}
			else
			{
				if (CurrentWeapon->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory != EWeaponCategory::Ewc_MeleeWeapons &&
					PlayerCharacter->GetWeaponInventory()->HasWeaponInCategory(EWeaponCategory::Ewc_MeleeWeapons))
				{
					// PlayerCharacter->GetWeaponInventory()->SwapToBackWeapon(CurrentWeapon, PlayerCharacter, EWeaponCategory::Ewc_MeleeWeapons);
					HandleMeleeActionPressed();
					SendInputActionToASC(true, EGASAbilityInputID::Attack);
				}
				else
				{
					if (!PlayerCharacter->GetWeaponInventory()->HasWeaponInCategory(EWeaponCategory::Ewc_MeleeWeapons) &&
						CurrentWeapon->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory != EWeaponCategory::Ewc_MeleeWeapons)
					{
						UE_LOG(LogTemp, Log, TEXT("Character should aim or focus for fire!"));
					}
					else
					{
						SendInputActionToASC(true, EGASAbilityInputID::Attack);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Character has no weapon! input Action part"));
		}
	}
}

void UGASEnhancedInputComponent::HandleReloadActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Reload Ability fired"));
	SendInputActionToASC(true, EGASAbilityInputID::Reload);
}

void UGASEnhancedInputComponent::HandleDropActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Drop Ability fired"));
	SendInputActionToASC(true, EGASAbilityInputID::Drop);
}

void UGASEnhancedInputComponent::HandleRaycastActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Raycast Action Ability fired"));
	SendInputActionToASC(true, EGASAbilityInputID::RaycastAction);
}

void UGASEnhancedInputComponent::HandleProjectileActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Projectile Action Ability fired"));
	SendInputActionToASC(true, EGASAbilityInputID::ProjectileAction);
}

void UGASEnhancedInputComponent::HandleMeleeActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Melee Action Ability fired"));
	SendInputActionToASC(true, EGASAbilityInputID::MeleeAction);
}

void UGASEnhancedInputComponent::HandleToggleFireModeActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Toggle Fire Mode Ability fired"));
	SendInputActionToASC(true, EGASAbilityInputID::ToggleFireMode);
}

void UGASEnhancedInputComponent::HandleSprintActionHold()
{
	UE_LOG(LogTemp, Log, TEXT("Sprinting..."));
	SendInputActionToASC(true, EGASAbilityInputID::Sprint);
}

void UGASEnhancedInputComponent::HandleAimingButtonPressed()
{
	if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetOwner()))
	{
		if (const AWeaponBase* CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon())
		{
			if (CurrentWeapon->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_MeleeWeapons)
			{
				SendInputActionToASC(true, EGASAbilityInputID::HeavyAttack);
				UE_LOG(LogTemp, Log, TEXT("Heavy Attack Ability Pressed"));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Aiming"));
				SendInputActionToASC(true, EGASAbilityInputID::Aiming);
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Character has no weapon! input Action part"));
		}
	}

}

void UGASEnhancedInputComponent::HandleLockonButtonPressed()
{
	SendInputActionToASC(true, EGASAbilityInputID::Lockon);
	UE_LOG(LogTemp, Log, TEXT("Lockon Pressed"));
}

// -------------------------------------------------------------------------------


void UGASEnhancedInputComponent::HandleJumpActionReleased()
{
	SendInputActionToASC(false, EGASAbilityInputID::Jump);
}

void UGASEnhancedInputComponent::HandleInteractActionReleased()
{
	SendInputActionToASC(false, EGASAbilityInputID::Interact);
}

void UGASEnhancedInputComponent::HandleFireActionReleased()
{
	if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetOwner()))
	{
		if (const AWeaponBase* CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon())
		{
			if (CurrentWeapon->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_RaycastWeapons &&
				PlayerCharacter->GetCharacterCameraComponent()->IsAiming() || PlayerCharacter->GetLockonComponent()->IsLocked())
			{
				SendInputActionToASC(false, EGASAbilityInputID::Fire);
			}
			else
			{
				SendInputActionToASC(false, EGASAbilityInputID::Attack);
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Character has no weapon! input Action part"));
		}
	}
}

void UGASEnhancedInputComponent::HandleReloadActionReleased()
{
	SendInputActionToASC(false, EGASAbilityInputID::Reload);
}

void UGASEnhancedInputComponent::HandleDropActionReleased()
{
	SendInputActionToASC(false, EGASAbilityInputID::Drop);
}

void UGASEnhancedInputComponent::HandleRaycastActionReleased()
{
	UE_LOG(LogTemp, Log, TEXT("Raycas action Released..."));
	SendInputActionToASC(false, EGASAbilityInputID::RaycastAction);
}

void UGASEnhancedInputComponent::HandleProjectileActionReleased()
{
	SendInputActionToASC(false, EGASAbilityInputID::ProjectileAction);
}

void UGASEnhancedInputComponent::HandleMeleeActionReleased()
{
	SendInputActionToASC(false, EGASAbilityInputID::MeleeAction);
}

void UGASEnhancedInputComponent::HandleToggleFireModeActionReleased()
{
	SendInputActionToASC(false, EGASAbilityInputID::ToggleFireMode);
	UE_LOG(LogTemp, Log, TEXT("Toggel Fire released..."));
}

void UGASEnhancedInputComponent::HandleSprintActionReleased()
{
	SendInputActionToASC(false, EGASAbilityInputID::Sprint);
	UE_LOG(LogTemp, Log, TEXT("Sprinting Released..."));
}

void UGASEnhancedInputComponent::HandleAimingButtonReleased()
{
	if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetOwner()))
	{
		if (const AWeaponBase* CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon())
		{
			if (CurrentWeapon->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_MeleeWeapons)
			{
				SendInputActionToASC(false, EGASAbilityInputID::HeavyAttack);
				UE_LOG(LogTemp, Log, TEXT("Heavy Attack Ability Released"));
			}
			else
			{
				SendInputActionToASC(false, EGASAbilityInputID::Aiming);
				UE_LOG(LogTemp, Log, TEXT("Aiming Released"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Character has no weapon! input Action part"));
		}
	}

}

void UGASEnhancedInputComponent::HandleLockonButtonReleased()
{
	SendInputActionToASC(false, EGASAbilityInputID::Lockon);
	UE_LOG(LogTemp, Log, TEXT("Lockon Released"));
}


void UGASEnhancedInputComponent::SendInputActionToASC(bool bIsPressed, const EGASAbilityInputID AbilityInputID) const
{
	ensure(AbilitySystemComponentPtr.IsValid());

	if (bIsPressed)
	{
		AbilitySystemComponentPtr->AbilityLocalInputPressed(static_cast<int32>(AbilityInputID));
	}
	else
	{
		AbilitySystemComponentPtr->AbilityLocalInputReleased(static_cast<int32>(AbilityInputID));
		UE_LOG(LogTemp, Warning, TEXT("Input Released: %d"), AbilityInputID);
	}
}

