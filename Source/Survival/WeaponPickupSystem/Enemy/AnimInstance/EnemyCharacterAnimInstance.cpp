// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalEnemyCharacter.h"

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
}
