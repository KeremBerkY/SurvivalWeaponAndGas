// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalSystemHUD.h"

#include "MainHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair/CrosshairBaseWidget.h"


void ASurvivalSystemHUD::BeginPlay()
{
	Super::BeginPlay();

	NotifyHUDReady();
	
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController) return;
	
	if (MainHUDWidgetClass)
	{
		MainHUDWidget = CreateWidget<UMainHUDWidget>(GetWorld(), MainHUDWidgetClass);
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
			ShowGameHUD();
		}
	}
}

void ASurvivalSystemHUD::NotifyHUDReady()
{
	OnHUDReady.Broadcast();
}

void ASurvivalSystemHUD::ShowGameHUD()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->ShowGameHUD();
	}
}

