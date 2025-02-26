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

	CharacterAbilitySystemComponentPtr = ASC; // MakeWeakObjectPtr
	CharacterAttributes = AttributeSet; // MakeWeakObjectPtr

	CharacterAbilitySystemComponentPtr->GetGameplayAttributeValueChangeDelegate(CharacterAttributes->GetHealthAttribute()).AddUObject(
		this, &UResourceComponent::HandleHealthChanged);

	CharacterAbilitySystemComponentPtr->GetGameplayAttributeValueChangeDelegate(CharacterAttributes->GetManaAttribute()).AddUObject(
		this, &UResourceComponent::HandleManaChanged);

	CharacterAbilitySystemComponentPtr->GetGameplayAttributeValueChangeDelegate(CharacterAttributes->GetStaminaAttribute()).AddUObject(
		this, &UResourceComponent::HandleStaminaChanged);

	
	BroadCastInitialResourceValues();

	UE_LOG(LogTemp, Log, TEXT("HealthComponent successfully initialized with GAS."));
}

void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// BroadCastInitialResourceValues();
	
}

void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UResourceComponent::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	if (CharacterAttributes.IsValid())
	{
		float NewHealth = Data.NewValue;
		float MaxHealth = CharacterAttributes->GetMaxHealth();

		CheckHealthState(NewHealth, MaxHealth);
		
		OnHealthChanged.Broadcast(NewHealth, MaxHealth);
	}
}

void UResourceComponent::HandleManaChanged(const FOnAttributeChangeData& Data)
{
	if (CharacterAttributes.IsValid())
	{
		float NewMana = Data.NewValue;
		float MaxMana = CharacterAttributes->GetMaxMana();
		
		OnManaChanged.Broadcast(NewMana, MaxMana);
	}
}

void UResourceComponent::HandleStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (CharacterAttributes.IsValid())
	{
		float NewStamina = Data.NewValue;
		float MaxStamina = CharacterAttributes->GetMaxStamina();

		OnStaminaChanged.Broadcast(NewStamina, MaxStamina);
	}
}

void UResourceComponent::BroadCastInitialResourceValues()
{
	if (CharacterAttributes.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Resource Values Health, Mana, Stamina Initialized."));
		InitializeHealthValues();
		InitializeManaValues();
		InitializeStaminaValues();
		
	}
}

void UResourceComponent::InitializeHealthValues()
{
	float CurrentHealth = CharacterAttributes->GetHealth();
	float MaxHealth = CharacterAttributes->GetMaxHealth();

	UE_LOG(LogTemp, Log, TEXT("health: %f & maxhealth: %f "), CurrentHealth, MaxHealth);

	CheckHealthState(CurrentHealth, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UResourceComponent::InitializeManaValues()
{
	float CurrentMana = CharacterAttributes->GetMana();
	float MaxMana = CharacterAttributes->GetMaxMana();

	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UResourceComponent::InitializeStaminaValues()
{
	float CurrentStamina = CharacterAttributes->GetStamina();
	float MaxStamina = CharacterAttributes->GetMaxStamina();

	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

float UResourceComponent::GetLowHealthThreshold() const
{
	return CharacterAttributes.IsValid() ? CharacterAttributes->GetMaxHealth() * 0.25f : 0.f;
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
