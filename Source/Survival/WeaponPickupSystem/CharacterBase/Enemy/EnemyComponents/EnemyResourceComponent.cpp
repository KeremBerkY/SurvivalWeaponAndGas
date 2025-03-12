// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyResourceComponent.h"

#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"


UEnemyResourceComponent::UEnemyResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UEnemyResourceComponent::InitializeWithGAS(UCharacterAbilitySystemComponent* ASC, UCharacterAttributeSet* AttributeSet)
{
	if (!ASC || !AttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthComponent: Invalid AbilitySystemComponent or AttributeSet"));
		return;
	}

	if (CharacterAbilitySystemComponentPtr.IsValid() && CharacterAbilitySystemComponentPtr == ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthComponent: AbilitySystemComponent is already initialized."));
		return;
	}

	CharacterAbilitySystemComponentPtr = MakeWeakObjectPtr(ASC);
	CharacterAttributesPtr = MakeWeakObjectPtr(AttributeSet);

	CharacterAbilitySystemComponentPtr->GetGameplayAttributeValueChangeDelegate(CharacterAttributesPtr->GetHealthAttribute()).AddUObject(
		this, &UEnemyResourceComponent::HandleHealthChanged);
	
	BroadCastInitialResourceValues();

	UE_LOG(LogTemp, Log, TEXT("HealthComponent successfully initialized with GAS."));
}

void UEnemyResourceComponent::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	if (CharacterAttributesPtr.IsValid())
	{
		float NewHealth = Data.NewValue;
		float MaxHealth = CharacterAttributesPtr->GetMaxHealth();

		// CheckHealthState(NewHealth, MaxHealth);
		
		OnEnemyHealthChanged.Broadcast(NewHealth, MaxHealth);
	}
}

void UEnemyResourceComponent::BroadCastInitialResourceValues()
{
	if (CharacterAttributesPtr.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Enemy Resource Values Health Initialized."));
		InitializeHealthValues();
	}
}

void UEnemyResourceComponent::InitializeHealthValues()
{
	float CurrentHealth = CharacterAttributesPtr->GetHealth();
	float MaxHealth = CharacterAttributesPtr->GetMaxHealth();

	UE_LOG(LogTemp, Log, TEXT("health: %f & maxhealth: %f "), CurrentHealth, MaxHealth);

	// CheckHealthState(CurrentHealth, MaxHealth);

	OnEnemyHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}
