// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUDWidget.h"


#include "Survival/WeaponPickupSystem/UserInterface/Crosshair/CrosshairBaseWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/Interaction/InteractionWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/WeaponHeat/WeaponHeatBar.h"


void UGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (HeatProgressBar)
	{
		HeatProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (CrosshairBase)
	{
		CrosshairBase->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}



