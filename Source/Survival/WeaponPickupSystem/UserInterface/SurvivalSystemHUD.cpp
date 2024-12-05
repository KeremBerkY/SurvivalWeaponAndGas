// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalSystemHUD.h"

#include "MainHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Interaction/InteractionWidget.h"
#include "Crosshair/CrosshairBaseWidget.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterStateComponent.h"
#include "Survival/WeaponPickupSystem/PickupSystem/BasePickup.h"
#include "Survival/WeaponPickupSystem/PickupSystem/Interfaces/InteractionInterface.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"
#include "WeaponInventory/WeaponInventoryWidget.h"
#include "WidgetController/WeaponInventoryController/WeaponInventoryWidgetController.h"


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

