// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"
#include "ResourceComponent.generated.h"


struct FOnAttributeChangeData;
class UCharacterAttributeSet;
class UCharacterAbilitySystemComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthRegenerated, float, RegenAmount, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFullyHealed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, float, NewMana, float, MaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRageChanged, float, NewRage, float, MaxRage);

/*
 *
 * TODO: BU SINIFA PARENT YAP HEM ENEMY HEM DE KARAKTER KULLANABİLSİN! HEALTH İKİSİNDE DE OLSUN GERİSİNİ SINIFLARA GÖRE ŞEKİLLENDİRİRSİN.
 * 
 */


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UResourceComponent();

	void InitializeWithGAS(UCharacterAbilitySystemComponent* ASC, UCharacterAttributeSet* AttributeSet);
	UFUNCTION()
	void BindAndInitializeRageResource();
	
// Delegates for broadcasting Health events
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health") // UI ile karakter üzerinde +'lar çıkabilir ama kritik seviyedeyken!
	FOnHealthRegenerated OnHealthRegenerated;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnFullyHealed OnFullyHealed;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnLowHealth OnLowHealth;
	
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintAssignable, Category = "Mana")
	FOnManaChanged OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Rage")
	FOnRageChanged OnRageChanged;

private:
	
	void HandleHealthChanged(const FOnAttributeChangeData& Data); 
	void HandleManaChanged(const FOnAttributeChangeData& Data);
	void HandleRageChanged(const FOnAttributeChangeData& Data);

	UFUNCTION()
	void CheckHealthState(float NewHealth, float MaxHealth);
	
	UFUNCTION()
	void BroadCastInitialResourceValues();

	UFUNCTION()
	void InitializeHealthValues();
	UFUNCTION()
	void InitializeManaValues();
	UFUNCTION()
	void InitializeRageValues();
	
	float GetLowHealthThreshold() const;
	
	UPROPERTY()
	TWeakObjectPtr<UCharacterAbilitySystemComponent> CharacterAbilitySystemComponentPtr;
	UPROPERTY()
	TWeakObjectPtr<UCharacterAttributeSet> CharacterAttributesPtr;
	
	bool bRegenerationActive;
	
};
