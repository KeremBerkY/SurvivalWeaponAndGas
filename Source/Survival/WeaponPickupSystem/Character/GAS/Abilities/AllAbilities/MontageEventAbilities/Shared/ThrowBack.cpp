// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowBack.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Survival/WeaponPickupSystem/CharacterBase/Enemy/SurvivalEnemyCharacter.h"

void UThrowBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid() || !TriggerEventData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	AActor* EnemyActor = ActorInfo->AvatarActor.Get();
	if (!EnemyActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	EnemyCharacterPtr = MakeWeakObjectPtr(Cast<ASurvivalEnemyCharacter>(EnemyActor));
	
	const auto EventInstigator = TriggerEventData->Instigator;
	UPrimitiveComponent* EnemyRoot = Cast<UPrimitiveComponent>(EnemyActor->GetRootComponent());
	if (EventInstigator)
	{
		FVector InstigatorForwardVector = EventInstigator->GetActorForwardVector();

		FVector KnockBackForce = (InstigatorForwardVector * 1000.f) + FVector(0, 0, 500.f);
		
		if (EnemyRoot)
		{
			EnemyRoot->SetSimulatePhysics(true);
			
			EnemyRoot->AddImpulse(KnockBackForce, NAME_None, true);
		}
		// else if (EnemyCharacterPtr.IsValid())
		// {
		// 	
		// 	UCharacterMovementComponent* MovementComp = EnemyCharacterPtr->GetCharacterMovement();
		// 	if (MovementComp)
		// 	{
		// 		MovementComp->Launch(FVector(KnockBackForce));
		// 	}
		// }
	}

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, EventInstigator, EnemyRoot]()
	{
		// MyParam burada capture edildi
		SetEnemyRotationToOriginal(EventInstigator, EnemyRoot);
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.0f, false);

	// TODO: EndAbility çağrıldığı için bizim timer devreye girmiyor! ve Enemy roptasyonuna lookattarget yap karaktere baksın düz durduktan sonra. Bunlara çare bul!!!
	
}


void UThrowBack::SetEnemyRotationToOriginal(TObjectPtr<const AActor> Instigator, UPrimitiveComponent* EnemyRoot)
{
	if (EnemyRoot)
	{
		EnemyRoot->SetSimulatePhysics(false);
	}
	
	FVector TargetLocation = Instigator->GetActorLocation();
	FVector EnemyLocation = GetEnemyCharacterFromActorInfo()->GetActorLocation();
	
	FVector DirectionToTarget = (TargetLocation - EnemyLocation).GetSafeNormal();
	
	FRotator TargetRotation = DirectionToTarget.Rotation();

	FRotator CurrentRotation = EnemyCharacterPtr->GetActorRotation();
	FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);

	EnemyCharacterPtr->SetActorRotation(SmoothedRotation);
	AdjustCharacterToGround(EnemyCharacterPtr.Get());

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
}

void UThrowBack::AdjustCharacterToGround(ASurvivalEnemyCharacter* EnemyCharacter)
{
	if (!EnemyCharacter || !EnemyCharacter->GetCharacterMovement())
	{
		UE_LOG(LogTemp, Error, TEXT("AdjustCharacterToGround failed: EnemyCharacter or CharacterMovement is NULL!"));
		return;
	}
	
	FHitResult FloorHit;
	FVector StartLocation = EnemyCharacter->GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, 200.f);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(EnemyCharacter);

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.0f, 0, 2.0f);

	if (GetWorld()->LineTraceSingleByChannel(FloorHit, StartLocation, EndLocation, ECC_Visibility, TraceParams))
	{
		DrawDebugSphere(GetWorld(), FloorHit.ImpactPoint, 10.f, 12, FColor::Green, false, 2.0f);
		
		FVector NewLocation = FloorHit.ImpactPoint + FVector(0.f, 0.f, EnemyCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		EnemyCharacter->SetActorLocation(NewLocation);
	}
	
}


void UThrowBack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	
}
