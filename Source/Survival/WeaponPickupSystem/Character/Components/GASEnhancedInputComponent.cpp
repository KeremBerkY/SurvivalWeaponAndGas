// Fill out your copyright notice in the Description page of Project Settings.


#include "GASEnhancedInputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Survival/SurvivalCharacter.h"


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
	UE_LOG(LogTemp, Log, TEXT("Fire Ability fired"));
	SendInputActionToASC(true, EGASAbilityInputID::Fire);
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


void UGASEnhancedInputComponent::HandleJumpActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::Jump);
}

void UGASEnhancedInputComponent::HandleInteractActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::Interact);
}

void UGASEnhancedInputComponent::HandleFireActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::Fire);
}

void UGASEnhancedInputComponent::HandleReloadActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::Reload);
}

void UGASEnhancedInputComponent::HandleDropActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::Drop);
}

void UGASEnhancedInputComponent::HandleRaycastActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::RaycastAction);
}

void UGASEnhancedInputComponent::HandleProjectileActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::ProjectileAction);
}

void UGASEnhancedInputComponent::HandleMeleeActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::MeleeAction);
}

void UGASEnhancedInputComponent::HandleToggleFireModeActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::ToggleFireMode);
}

void UGASEnhancedInputComponent::HandleSprintActionReleased()
{
	SendInputActionToASC(true, EGASAbilityInputID::Sprint);
	UE_LOG(LogTemp, Log, TEXT("Released..."));
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
	}
}

