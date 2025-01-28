// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SurvivalAbilitySystemLibrary.generated.h"

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
	
	static UCharacterAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category = "Survival|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);
	
	UFUNCTION(BlueprintCallable, Category = "Survival|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	// UFUNCTION(BlueprintPure)
	// static UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponent(const AActor* Actor);
};
