// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalCharacterBase.h"
#include "SurvivalEnemyCharacter.generated.h"

class USelectedWidgetComponent;
class ULockedWidgetComponent;
class UDataAsset_EnemyStartUpData;

UCLASS()
class SURVIVAL_API ASurvivalEnemyCharacter : public ASurvivalCharacterBase
{
	GENERATED_BODY()

public:
	ASurvivalEnemyCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, Category = "Components | Lockon")
	ULockedWidgetComponent* LockedWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components | Lockon")
	USelectedWidgetComponent* SelectedWidgetComponent;
private:
	void InitEnemyStartupData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataAsset_EnemyStartUpData> EnemyStartUpData;

public:
	FORCEINLINE ULockedWidgetComponent* GetLockedWidgetComponent() const { return LockedWidgetComponent; }
	FORCEINLINE USelectedWidgetComponent* GetSelectedWidgetComponent() const { return SelectedWidgetComponent; }
};
