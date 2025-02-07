// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalEnemyGameplayAbility.h"

#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalEnemyCharacter.h"

ASurvivalEnemyCharacter* USurvivalEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedEnemyCharacter.IsValid())
	{
		CachedEnemyCharacter = Cast<ASurvivalEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedEnemyCharacter.IsValid()? CachedEnemyCharacter.Get() : nullptr;
}
