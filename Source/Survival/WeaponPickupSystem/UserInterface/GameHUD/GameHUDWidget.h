// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Survival/WeaponPickupSystem/PickupSystem/BasePickup.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "GameHUDWidget.generated.h"

class UWeaponHeatBar;
class UResourceWidget;
class UWeaponInventoryWidget;
class UCrosshairBaseWidget;
class UInteractionWidget;
class AWeaponBase;

/**
 * 
 */
UCLASS()
class SURVIVAL_API UGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE UInteractionWidget* GetInteractionWidget() const { return InteractionWidget; }
	FORCEINLINE UWeaponInventoryWidget* GetWeaponInventoryWidget() const { return WeaponInventoryWidget; }
	FORCEINLINE UResourceWidget* GetHealthWidget() const { return ResourceWidget; }
	FORCEINLINE UWeaponHeatBar* GetHeatProgressBar() const { return HeatProgressBar; }

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UInteractionWidget* InteractionWidget;

	UPROPERTY(meta = (BindWidget))
	UWeaponInventoryWidget* WeaponInventoryWidget;
	
	UPROPERTY(meta = (BindWidget))
	UResourceWidget* ResourceWidget;
	
	UPROPERTY(meta = (BindWidget))
	UWeaponHeatBar* HeatProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	UCrosshairBaseWidget* CrosshairBase;
};
