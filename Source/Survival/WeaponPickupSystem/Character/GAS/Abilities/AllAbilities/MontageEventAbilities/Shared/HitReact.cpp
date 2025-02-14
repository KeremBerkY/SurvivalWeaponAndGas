// Fill out your copyright notice in the Description page of Project Settings.


#include "HitReact.h"
#include "Kismet/KismetMathLibrary.h"

#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalEnemyCharacter.h"

UHitReact::UHitReact()
{
	bHasHitReactMontagesToPlay = true;
	FaceAttacker = true;
	
}

void UHitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (FaceAttacker)
	{
		const auto EventInstigator = TriggerEventData->Instigator;

		const FRotator LookAtRotation= UKismetMathLibrary::FindLookAtRotation(GetEnemyCharacterFromActorInfo()->GetActorLocation(), EventInstigator->GetActorLocation());
		GetEnemyCharacterFromActorInfo()->SetActorRotation(LookAtRotation);
	}


	if (bHasHitReactMontagesToPlay)
	{
		int32 RandomIndex = FMath::RandRange(0, MontagesToPlay.Num() - 1);
		AbilityMontage = MontagesToPlay[RandomIndex];
		
		if (USkeletalMeshComponent* MeshComp = GetEnemyCharacterFromActorInfo()->GetMesh())
		{
			MeshComp->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.0f);
		}
		StartAnimMontage();
	}
	else
	{
		if (USkeletalMeshComponent* MeshComp = GetEnemyCharacterFromActorInfo()->GetMesh())
		{
			MeshComp->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.0f);
		}

		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UHitReact::EndAbility, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		
		GetWorld()->GetTimerManager().SetTimer(HitReactTimerHandle, TimerDel, 0.2f, false);
	}
}

void UHitReact::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);
	
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &UHitReact::EndAbility, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		
	GetWorld()->GetTimerManager().SetTimer(HitReactTimerHandle, TimerDel, 0.2f, false);

}

void UHitReact::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UHitReact::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UHitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (USkeletalMeshComponent* MeshComp = GetEnemyCharacterFromActorInfo()->GetMesh())
	{
		MeshComp->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 0.0f);
	}
}
