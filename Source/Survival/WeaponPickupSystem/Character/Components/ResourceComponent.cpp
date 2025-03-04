// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceComponent.h"

#include "AbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"


UResourceComponent::UResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UResourceComponent::InitializeWithGAS(UCharacterAbilitySystemComponent* ASC, UCharacterAttributeSet* AttributeSet)
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

	CharacterAbilitySystemComponentPtr = MakeWeakObjectPtr(ASC); // MakeWeakObjectPtr
	CharacterAttributesPtr = MakeWeakObjectPtr(AttributeSet); // MakeWeakObjectPtr

	CharacterAbilitySystemComponentPtr->GetGameplayAttributeValueChangeDelegate(CharacterAttributesPtr->GetHealthAttribute()).AddUObject(
		this, &UResourceComponent::HandleHealthChanged);

	CharacterAbilitySystemComponentPtr->GetGameplayAttributeValueChangeDelegate(CharacterAttributesPtr->GetManaAttribute()).AddUObject(
		this, &UResourceComponent::HandleManaChanged);

	CharacterAbilitySystemComponentPtr->GetGameplayAttributeValueChangeDelegate(CharacterAttributesPtr->GetCurrentRageAttribute()).AddUObject(
		this, &UResourceComponent::HandleRageChanged);

	
	BroadCastInitialResourceValues();

	UE_LOG(LogTemp, Log, TEXT("HealthComponent successfully initialized with GAS."));
}

void UResourceComponent::BindAndInitializeRageResource()
{
	if (!CharacterAbilitySystemComponentPtr.IsValid() || !CharacterAttributesPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthComponent: Invalid AbilitySystemComponent or AttributeSet"));
		return;
	}

	CharacterAbilitySystemComponentPtr->GetGameplayAttributeValueChangeDelegate(CharacterAttributesPtr->GetStaminaAttribute()).AddUObject(
	this, &UResourceComponent::HandleRageChanged);

	InitializeRageValues();
}

void UResourceComponent::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	if (CharacterAttributesPtr.IsValid())
	{
		float NewHealth = Data.NewValue;
		float MaxHealth = CharacterAttributesPtr->GetMaxHealth();

		CheckHealthState(NewHealth, MaxHealth);
		
		OnHealthChanged.Broadcast(NewHealth, MaxHealth);
	}
}

void UResourceComponent::HandleManaChanged(const FOnAttributeChangeData& Data)
{
	if (CharacterAttributesPtr.IsValid())
	{
		float NewMana = Data.NewValue;
		float MaxMana = CharacterAttributesPtr->GetMaxMana();
		
		OnManaChanged.Broadcast(NewMana, MaxMana);
	}
}

void UResourceComponent::HandleRageChanged(const FOnAttributeChangeData& Data)
{
	// if (CharacterAttributes.IsValid())
	// {
	// 	float NewStamina = Data.NewValue;
	// 	float MaxStamina = CharacterAttributes->GetMaxStamina();
	//
	// 	OnStaminaChanged.Broadcast(NewStamina, MaxStamina);
	// }

	if (CharacterAttributesPtr.IsValid())
	{
		float NewRage = Data.NewValue;
		float MaxRage = CharacterAttributesPtr->GetMaxRage();

		OnRageChanged.Broadcast(NewRage, MaxRage);
	}
}

void UResourceComponent::BroadCastInitialResourceValues()
{
	if (CharacterAttributesPtr.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Resource Values Health, Mana, Stamina Initialized."));
		InitializeHealthValues();
		InitializeManaValues();
		InitializeRageValues();
	}
}

void UResourceComponent::InitializeHealthValues()
{
	float CurrentHealth = CharacterAttributesPtr->GetHealth();
	float MaxHealth = CharacterAttributesPtr->GetMaxHealth();

	UE_LOG(LogTemp, Log, TEXT("health: %f & maxhealth: %f "), CurrentHealth, MaxHealth);

	CheckHealthState(CurrentHealth, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UResourceComponent::InitializeManaValues()
{
	float CurrentMana = CharacterAttributesPtr->GetMana();
	float MaxMana = CharacterAttributesPtr->GetMaxMana();

	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UResourceComponent::InitializeRageValues()
{
	// float CurrentStamina = CharacterAttributes->GetStamina();
	// float MaxStamina = CharacterAttributes->GetMaxStamina();
	//
	// OnRageChanged.Broadcast(CurrentStamina, MaxStamina);

	float CurrentRage = CharacterAttributesPtr->GetCurrentRage();
	float MaxRage = CharacterAttributesPtr->GetMaxRage();

	OnRageChanged.Broadcast(CurrentRage, MaxRage);
}

float UResourceComponent::GetLowHealthThreshold() const
{
	return CharacterAttributesPtr.IsValid() ? CharacterAttributesPtr->GetMaxHealth() * 0.25f : 0.f;
}

void UResourceComponent::CheckHealthState(float NewHealth, float MaxHealth)
{
	if (NewHealth <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character has died!"));

		OnDeath.Broadcast();
		return;
	}

	if (FMath::IsNearlyEqual(NewHealth, MaxHealth, 0.01f))
	{
		UE_LOG(LogTemp, Log, TEXT("Character is fully healed!"));
		
		OnFullyHealed.Broadcast(); // TODO: Armor artmaya başlayabilir?
		return;
	}
	
	if (NewHealth < GetLowHealthThreshold())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character health is critically low!"));

		OnLowHealth.Broadcast(); // TODO: Görüş kısalabilir, etraf kararabilir?
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Character health is stable: %f / %f"), NewHealth, MaxHealth);
}
