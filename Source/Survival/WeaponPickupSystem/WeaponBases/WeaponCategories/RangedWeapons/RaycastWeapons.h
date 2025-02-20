// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangedWeapon.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "RaycastWeapons.generated.h"

class UWeaponTargetingComponent;
class AEnemyBase;
class UFireModeBaseComponent;
class URaycastWeaponUIHandler;
class UWeaponAnimationsComponent;
class URaycastWeaponData;
class URaycastEffectManagerComponent;
class UHeatComponent;

// Initialize
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitializedComponents); 
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitializedAnims);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireMade); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFirePlayMontage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateHeat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImpact, const FHitResult&, HitResult);

DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*)

UCLASS()
class SURVIVAL_API ARaycastWeapons : public ARangedWeapon
{
	GENERATED_BODY()

public:
	ARaycastWeapons();

	/* DELEGATE */
	FOnInitializedComponents OnInitializedComponents;
	// FOnInitializedAnims OnInitializedAnims;
	
	FOnFireMade OnFireMade;
	FOnFirePlayMontage OnFirePlayMontage;
	FOnImpact OnImpact;
	FOnUpdateHeat UpdateHeat;

	FOnTargetInteractedDelegate OnRayHitTarget;
	
	/* Getters / Setters */
	FORCEINLINE URaycastWeaponData* GetRaycastWeaponDataAsset() const { return RaycastWeaponData; }
	FORCEINLINE UHeatComponent* GetHeatComponent() const { return HeatComponent; }
	FORCEINLINE URaycastWeaponUIHandler* GetRaycastWeaponUIHandler() const { return  RaycastWeaponUIHandler; }
	FORCEINLINE TArray<UFireModeBaseComponent*> GetFireModeComponents() const { return FireModeComponents; }
	FORCEINLINE UFireModeBaseComponent* GetCurrentFireModeComponent() const { return CurrentFireModeComponent; }
	FORCEINLINE void SetCurrentFireModeComponent(UFireModeBaseComponent* NewFireModeComponent) { CurrentFireModeComponent = NewFireModeComponent;}
	FORCEINLINE int32 GetCurrentFireModeIndex() const { return CurrentFireModeIndex; }
	FORCEINLINE void SetCurrentFireModeIndex(const float Index) { CurrentFireModeIndex = Index; }

	virtual void PerformFire() override;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void Attack() override;
	virtual void EndAttack() override;
	void DrawDebugVisuals(const FVector& Start, const FVector& End, const FHitResult& HitResult) const;
	
	virtual void AddFireModes();

	void CalculateTargetPoint();
	
	UPROPERTY(VisibleAnywhere, Category = "EffectManager")
	URaycastEffectManagerComponent* EffectManagerComponent;

	UPROPERTY()
	TArray<UFireModeBaseComponent*> FireModeComponents;

	UPROPERTY()
	UFireModeBaseComponent* CurrentFireModeComponent;

	// TWeakObjectPtr<ASurvivalCharacter> PlayerCharacterPtr;
	
	int32 CurrentFireModeIndex;
	
	FVector RaycastMuzzleLocation;
	FVector RaycastMuzzleForward;
	FVector RaycastTraceEnd;

private:
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	float TrailEffectDistance = 10000.f;

	UPROPERTY()
	URaycastWeaponData* RaycastWeaponData;

	UPROPERTY(VisibleAnywhere, Category = "EffectManager")
	UWeaponAnimationsComponent* WeaponAnimationsComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "UIHandler")
	URaycastWeaponUIHandler* RaycastWeaponUIHandler;

	UPROPERTY(VisibleAnywhere, Category = "Heat")
	UHeatComponent* HeatComponent;
	
	FTimerHandle RecoilCooldownTimerHandle;
	
	UPROPERTY(VisibleAnywhere, Category = "FireMode")
	USingleShotModeComponent* SingleShotModeComponent;

	UPROPERTY(VisibleAnywhere, Category = "FireMode")
	UWeaponTargetingComponent* WeaponTargetingComponent;

	// DENEME
	TWeakObjectPtr<AEnemyBase> EnemyPtr;

	
};
