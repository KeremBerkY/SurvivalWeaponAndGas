// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Survival/WeaponPickupSystem/PickupSystem/BasePickup.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "MainHUDWidget.generated.h"

// struct FInteractableData;

class UGameHUDWidget;
class AWeaponBase;
class UInteractionWidget;
class UWeaponInventoryWidget;
class UCrosshairBaseWidget;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FORCEINLINE UGameHUDWidget* GetGameHUDWidget() const { return GameHUDWidget; }
	
	void ShowGameHUD();
	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UGameHUDWidget* GameHUDWidget;
	
	
};
