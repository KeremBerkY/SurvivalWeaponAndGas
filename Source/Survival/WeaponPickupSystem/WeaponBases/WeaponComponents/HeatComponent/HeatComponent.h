// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeatComponent.generated.h"


class ARaycastWeapons;
class URaycastWeaponData;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateHeatBar, float, Heat, float, MaxHeat);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoveHeat, float, Heat, float, MaxHeat);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UHeatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHeatComponent();

	// FOnUpdateHeatBar UpdateHeatBar;
	// FOnRemoveHeat RemoveHeat;

	FORCEINLINE float GetCurrentHeat() const { return CurrentHeat; }
	FORCEINLINE void SetCurrentHeat(const float NewHeat) { CurrentHeat = NewHeat; }
	FORCEINLINE bool IsOverHeated() const { return bIsOverHeated; }
	// FORCEINLINE FTimerHandle GetCoolingTimerHandle()

	UFUNCTION()
	void UpdateHeat();

	void ClearHeatCoolerTimer();
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void InitializeHeat();

	bool IsOverheated() const;
	bool CompleteCooling() const;

	bool CanStartCooling() const;
	void StartCooling();
	void ApplyCooling();

	void ResetHeatState();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heat")
	float CurrentHeat;

	UPROPERTY()
	ARaycastWeapons* Weapon;
	
	UPROPERTY()
	URaycastWeaponData* WeaponDataAsset;

	// FTimerHandle HeatTimerHandle;
	FTimerHandle CoolingTimerHandle;

	bool bIsCoolingDown;
	bool bIsOverHeated;

	float LastFiredTime;

};