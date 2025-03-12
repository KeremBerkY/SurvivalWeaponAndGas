// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ActivateAbilityByTag.h"

#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/CharacterBase/Enemy/SurvivalEnemyCharacter.h"

void UBTTask_ActivateAbilityByTag::OnEnemyExecuteTask(UBehaviorTreeComponent& OwnerComp)
{
	Super::OnEnemyExecuteTask(OwnerComp);

	OwningEnemyCharacter->GetCharacterAbilitySystemComponent()->TryActivateAbilityByTag(AbilityTagToActivate);
}
