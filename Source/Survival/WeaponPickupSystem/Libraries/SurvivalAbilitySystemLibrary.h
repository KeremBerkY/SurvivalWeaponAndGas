// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Survival/WeaponPickupSystem/SurvivalTypes/SurvivalEnumTypes.h"
#include "SurvivalAbilitySystemLibrary.generated.h"

class USurvivalCharacterCombatComponent;
class UPawnCombatComponent;
class UCharacterAbilitySystemComponent;
class UAbilitySystemComponent;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class SURVIVAL_API USurvivalAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static UCharacterClassInfo* GetCharacterClassDefaultInfo(const UObject* WorldContextObject);
	
	static UCharacterAbilitySystemComponent* NativeGetCharacterASCFromActor(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category = "Survival|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);
	
	UFUNCTION(BlueprintCallable, Category = "Survival|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);
	static USurvivalCharacterCombatComponent* NativeGetSurvivalCharacterCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Survival|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType "))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType);

	UFUNCTION(BlueprintCallable, Category = "Survival|FunctionLibrary", meta = (DisplayName = "Get Survival Character Combat Component From Actor", ExpandEnumAsExecs = "OutValidType "))
	static USurvivalCharacterCombatComponent* BP_GetSurvivalCharacterCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType);

	// UFUNCTION(BlueprintPure)
	// static UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponent(const AActor* Actor);
};
