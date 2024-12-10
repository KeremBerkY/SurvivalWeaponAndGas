// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDWidget.h"

#include "Components/WidgetSwitcher.h"
#include "Crosshair/CrosshairBaseWidget.h"
#include "GameHUD/GameHUDWidget.h"

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
