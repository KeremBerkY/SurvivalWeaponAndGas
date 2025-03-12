// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ToggleStrafingState.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/CharacterBase/Enemy/SurvivalEnemyCharacter.h"
#include "Survival/WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"

UBTTask_ToggleStrafingState::UBTTask_ToggleStrafingState()
{
	NodeName = TEXT("Native Toggle Strafing State");
	StrafingWalkSpeed = 200.f;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

void UBTTask_ToggleStrafingState::OnEnemyExecuteTask(UBehaviorTreeComponent& OwnerComp)
{
	Super::OnEnemyExecuteTask(OwnerComp);

	if (bShouldEnable)
	{
		if (OwningEnemyCharacter)
		{
			OwningEnemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
			if (bShouldChangeMaxWalkSpeed)
			{
				OwningEnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = StrafingWalkSpeed;
				USurvivalAbilitySystemLibrary::AddGameplayTagToActorIfNone(
					OwningEnemyCharacter,
					FGameplayTag::RequestGameplayTag(FName("Character.Enemy.Status.Strafing"))
				);
			}
			else
			{
				USurvivalAbilitySystemLibrary::AddGameplayTagToActorIfNone(
					OwningEnemyCharacter,
					FGameplayTag::RequestGameplayTag(FName("Character.Enemy.Status.Strafing"))
				);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter is not valid!"));
		}
	}
	else
	{
		OwningEnemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			
		bool bIsNearlyEqual = FMath::IsNearlyEqual(
			OwningEnemyCharacter->GetCharacterMovement()->MaxWalkSpeed,
			OwnerComp.GetBlackboardComponent()->GetValueAsFloat(InDefaultMaxWalkSpeedKey.SelectedKeyName)
		);

		float SelectedSpeed = bIsNearlyEqual ?
			OwningEnemyCharacter->GetCharacterMovement()->MaxWalkSpeed : OwnerComp.GetBlackboardComponent()->GetValueAsFloat(InDefaultMaxWalkSpeedKey.SelectedKeyName);

		OwningEnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = SelectedSpeed;

		USurvivalAbilitySystemLibrary::RemoveGameplayTagFromActorIfFound(
			OwningEnemyCharacter,
			FGameplayTag::RequestGameplayTag(FName("Character.Enemy.Status.Strafing"))
		);
	}
}
