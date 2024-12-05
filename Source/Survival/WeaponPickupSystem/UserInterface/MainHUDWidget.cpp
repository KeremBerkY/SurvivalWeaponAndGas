// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDWidget.h"

#include "Components/WidgetSwitcher.h"
#include "Crosshair/CrosshairBaseWidget.h"
#include "GameHUD/GameHUDWidget.h"
#include "Survival/WeaponPickupSystem/PickupSystem/BasePickup.h"
#include "WeaponInventory/WeaponInventoryWidget.h"
#include "Interaction/InteractionWidget.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (WidgetSwitcher && GameHUDWidget)
	{
		WidgetSwitcher->SetActiveWidget(GameHUDWidget);
	}
}

void UMainHUDWidget::ShowGameHUD()
{
	if (WidgetSwitcher && GameHUDWidget)
	{
		WidgetSwitcher->SetActiveWidget(GameHUDWidget);
	}
}
