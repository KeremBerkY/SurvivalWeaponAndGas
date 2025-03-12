// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"
#include "EnemyResourceComponent.generated.h"

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyHealthChanged, float, NewHealth, float, MaxHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UEnemyResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyResourceComponent();
	void InitializeWithGAS(UCharacterAbilitySystemComponent* ASC, UCharacterAttributeSet* AttributeSet);

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnEnemyHealthChanged OnEnemyHealthChanged;

private:
	void HandleHealthChanged(const FOnAttributeChangeData& Data); // Buna neden UFUNCTION yazınca hata alıyoruz.
	

	// UFUNCTION()
	// void CheckHealthState(float NewHealth, float MaxHealth);
	
	UFUNCTION()
	void BroadCastInitialResourceValues();

	UFUNCTION()
	void InitializeHealthValues();


	UPROPERTY()
	TWeakObjectPtr<UCharacterAbilitySystemComponent> CharacterAbilitySystemComponentPtr;
	UPROPERTY()
	TWeakObjectPtr<UCharacterAttributeSet> CharacterAttributesPtr;
};
