// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalCharacterBase.h"
#include "Survival/WeaponPickupSystem/Interfaces/EnemyDeathInterface.h"
#include "SurvivalEnemyCharacter.generated.h"

class UEnemyResourceComponent;
class UWidgetComponent;
class UEnemyCombatComponent;
class USelectedWidgetComponent;
class ULockedWidgetComponent;
class UDataAsset_EnemyStartUpData;

UCLASS()
class SURVIVAL_API ASurvivalEnemyCharacter : public ASurvivalCharacterBase
{
	GENERATED_BODY()

public:
	ASurvivalEnemyCharacter();

	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "Dead", meta = (DisplayName = "AfterEnemyDied"))
	virtual void OnAfterEnemyDied();

	UFUNCTION(BlueprintCallable, Category = "Survival|Combat")
	UEnemyCombatComponent* GetEnemyCombatComponent();

protected:
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;
	void InitializeResourceComponent();

	UPROPERTY(VisibleAnywhere, Category = "Components | Lockon")
	ULockedWidgetComponent* LockedWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components | Lockon")
	USelectedWidgetComponent* SelectedWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Enemy Combat" )
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyResourceComponent* EnemyResourceComponent;
	
	// UFUNCTION(BlueprintImplementableEvent, Category = "Dead", meta=(DisplayName = "OnEnemyDied"))
	// void BP_OnEnemyDied();
private:
	void InitEnemyStartupData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataAsset_EnemyStartUpData> EnemyStartUpData;

public:
	FORCEINLINE ULockedWidgetComponent* GetLockedWidgetComponent() const { return LockedWidgetComponent; }
	FORCEINLINE USelectedWidgetComponent* GetSelectedWidgetComponent() const { return SelectedWidgetComponent; }
};
