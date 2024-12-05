// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUDWidget.h"

#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"
#include "Survival/WeaponPickupSystem/UserInterface/Crosshair/CrosshairBaseWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/Interaction/InteractionWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/WeaponInventory/WeaponInventoryWidget.h"


void UGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

