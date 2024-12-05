// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UCharacterGameplayAbility;

USTRUCT()
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> SpecialAttributes;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UCharacterGameplayAbility>> StartingAbilities;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UCharacterGameplayAbility>> StartingPassives;
};


UCLASS()
class SURVIVAL_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FCharacterClassDefaultInfo> ClassDefaultInfoMap;
};
