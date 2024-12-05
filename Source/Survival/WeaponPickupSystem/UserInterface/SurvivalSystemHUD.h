// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SurvivalSystemHUD.generated.h"

class UInteractionWidgetController;
class UWeaponInventoryWidgetController;
class UCharacterStateComponent;
class ASurvivalCharacter;
class UMainHUDWidget;
struct FInteractableData;
class UInteractionWidget;
class UCrosshairBaseWidget;
class UWeaponInventoryWidget;
class AWeaponBase;



// USTRUCT(BlueprintType)
// struct FHUDCrosshairSettings // TEST
// {
// 	GENERATED_USTRUCT_BODY();
//
// 	FHUDCrosshairSettings() :
// 		  DefaultSpread(0),
// 		  RunningSpread(0),
// 		  SprintingSpread(0)
// 	{}
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
// 	float DefaultSpread;
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
// 	float RunningSpread;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
// 	float SprintingSpread;
// };

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHUDReady);

UCLASS()
class SURVIVAL_API ASurvivalSystemHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	FORCEINLINE UMainHUDWidget* GetMainHUDWidget() const { return MainHUDWidget; }

	void NotifyHUDReady();
	void ShowGameHUD();

	FOnHUDReady OnHUDReady;
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UMainHUDWidget* MainHUDWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets | Main HUD")
	TSubclassOf<UMainHUDWidget> MainHUDWidgetClass;


};
