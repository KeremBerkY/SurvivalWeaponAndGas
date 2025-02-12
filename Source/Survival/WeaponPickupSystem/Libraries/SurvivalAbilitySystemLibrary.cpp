// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/SurvivalGameMode.h"
#include "Survival/WeaponPickupSystem/SharedComponents/Combat/PawnCombatComponent.h"

UCharacterClassInfo* USurvivalAbilitySystemLibrary::GetCharacterClassDefaultInfo(const UObject* WorldContextObject)
{
	if (const ASurvivalGameMode* SurvivalGameMode = Cast<ASurvivalGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return SurvivalGameMode->GetCharacterClassDefaultInfo();
	}
	
	return nullptr;
}

UCharacterAbilitySystemComponent* USurvivalAbilitySystemLibrary::NativeGetCharacterASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UCharacterAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void USurvivalAbilitySystemLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UCharacterAbilitySystemComponent* ASC = NativeGetCharacterASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void USurvivalAbilitySystemLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UCharacterAbilitySystemComponent* ASC = NativeGetCharacterASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool USurvivalAbilitySystemLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UCharacterAbilitySystemComponent* ASC = NativeGetCharacterASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

UPawnCombatComponent* USurvivalAbilitySystemLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor)

	if (const IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

USurvivalCharacterCombatComponent* USurvivalAbilitySystemLibrary::NativeGetSurvivalCharacterCombatComponentFromActor(AActor* InActor)
{
	check(InActor)

	if (const IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return Cast<USurvivalCharacterCombatComponent>(PawnCombatInterface->GetPawnCombatComponent());
	}

	return nullptr;
}

UPawnCombatComponent* USurvivalAbilitySystemLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = CombatComponent ? EWarriorValidType::Valid : EWarriorValidType::Invalid;

	return CombatComponent; 
}

USurvivalCharacterCombatComponent* USurvivalAbilitySystemLibrary::BP_GetSurvivalCharacterCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType)
{
	USurvivalCharacterCombatComponent* SurvivalCharacterCombatComponent = NativeGetSurvivalCharacterCombatComponentFromActor(InActor);

	OutValidType = SurvivalCharacterCombatComponent ? EWarriorValidType::Valid : EWarriorValidType::Invalid;

	return SurvivalCharacterCombatComponent; 
}
