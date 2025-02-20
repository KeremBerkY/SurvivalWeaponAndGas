// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowBack.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalEnemyCharacter.h"

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

		FVector KnockBackForce = (-InstigatorForwardVector * 1000.f) + FVector(0, 0, 500.f);
		
		if (EnemyRoot)
		{
			EnemyRoot->SetSimulatePhysics(true);
			
			EnemyRoot->AddImpulse(KnockBackForce, NAME_None, true);
		}
		else if (EnemyCharacterPtr.IsValid())
		{
			// Karakter ise, Character Movement kullanarak itme uygula
			UCharacterMovementComponent* MovementComp = EnemyCharacterPtr->GetCharacterMovement();
			if (MovementComp)
			{
				MovementComp->Launch(FVector(KnockBackForce));
			}
		}
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
	// FRotator CurrentRotation = EnemyActor->GetActorRotation();
	// UPrimitiveComponent* EnemyRoot = Cast<UPrimitiveComponent>(EnemyCharacterPtr->GetRootComponent());
	if (EnemyRoot)
	{
		// 🛑 Fiziği Kapat
		EnemyRoot->SetSimulatePhysics(false);
	}
	
	FVector TargetLocation = Instigator->GetActorLocation();
	FVector EnemyLocation = GetEnemyCharacterFromActorInfo()->GetActorLocation();

	// ✅ Enemy’nin yönünü saldıran karaktere döndür
	FVector DirectionToTarget = (TargetLocation - EnemyLocation).GetSafeNormal();
    
	// **Eski yöntem: FindLookAtRotation yerine doğrudan Direction kullan**
	FRotator TargetRotation = DirectionToTarget.Rotation();

	// 🔄 Yumuşak dönüş (eğer ani dönüş istenmiyorsa)
	FRotator CurrentRotation = EnemyCharacterPtr->GetActorRotation();
	FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);

	// **Enemy'yi saldırana doğru döndür**
	EnemyCharacterPtr->SetActorRotation(SmoothedRotation);
	//
	// FRotator FixedRotation = FRotator(0.f, 0.f, 0.f);
	// EnemyCharacterPtr->SetActorRotation(FixedRotation);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
}


void UThrowBack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	
}
