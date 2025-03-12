// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedWidgetComponent.h"

#include "Survival/WeaponPickupSystem/UserInterface/Lockon/LockonUI.h"


ULockedWidgetComponent::ULockedWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetVisibility(false);
}

void ULockedWidgetComponent::ShowLockWidget()
{
	SetVisibility(true);
}

void ULockedWidgetComponent::HideLockWidget()
{
	SetVisibility(false);
}
