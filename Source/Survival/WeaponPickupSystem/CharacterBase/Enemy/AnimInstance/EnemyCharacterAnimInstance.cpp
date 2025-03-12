// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterAnimInstance.h"

#include "GameplayTagContainer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Survival/WeaponPickupSystem/CharacterBase/Enemy/SurvivalEnemyCharacter.h"
#include "Survival/WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"

void UEnemyCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	
	OwningEnemyCharacter = Cast<ASurvivalEnemyCharacter>(TryGetPawnOwner());

	if (OwningEnemyCharacter)
	{
		OwningMovementComponent = OwningEnemyCharacter->GetCharacterMovement();
			
	}
}

void UEnemyCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!OwningEnemyCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningEnemyCharacter->GetVelocity().Size();

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D()>0.f;

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningEnemyCharacter->GetVelocity(), OwningEnemyCharacter->GetActorRotation());
}

bool UEnemyCharacterAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (OwningEnemyCharacter)
	{
		return USurvivalAbilitySystemLibrary::NativeDoesActorHaveTag(OwningEnemyCharacter, TagToCheck);
	}

	return false;
}
