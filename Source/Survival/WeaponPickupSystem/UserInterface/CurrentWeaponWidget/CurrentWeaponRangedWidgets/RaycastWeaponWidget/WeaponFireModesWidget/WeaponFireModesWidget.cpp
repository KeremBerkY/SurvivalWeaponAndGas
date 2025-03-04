// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponFireModesWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UWeaponFireModesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// SetVisibility(ESlateVisibility::Hidden);
}

// void UWeaponFireModesWidget::SetFireModeState(int32 FireModeIndex)
// {
// 	if (!WidgetSwitcher) return;
//
// 	WidgetSwitcher->SetActiveWidgetIndex(FireModeIndex);
// }

void UWeaponFireModesWidget::SetWidgetVisibility(bool bVisible)
{
	if (bVisible)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWeaponFireModesWidget::SetFireModeState(EFireMode FireMode)
{
	if (!WidgetSwitcher) return;

	switch (FireMode)
	{
	case EFireMode::SingleShot:
		WidgetSwitcher->SetActiveWidget(SingleShotModeImage);
		break;

	case EFireMode::BurstShot:
		WidgetSwitcher->SetActiveWidget(BurstShotModeImage);
		break;

	case EFireMode::AutomaticShot:
		WidgetSwitcher->SetActiveWidget(AutomaticShotModeImage);
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid Widget State!"));
		break;
	}
}

void UWeaponFireModesWidget::SetNumberOfModesText(int32 TotalModeIndex)
{
	if (TotalModeIndex)
	{
		NumberOfModes->SetText(FText::AsNumber(TotalModeIndex));
	}
}
