// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalWidgetBase.h"

#include "SurvivalSizeBox.h"
#include "Styling/SlateTypes.h"
#include "Components/ProgressBar.h"

// void USurvivalWidgetBase::NativePreConstruct()
// {
// 	Super::NativePreConstruct();
//
//
// }

void USurvivalWidgetBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (SurvivalSizeBox)
	{
		SurvivalSizeBox->SetWidthHeightOverride(SurvivalSizeBox->DesiredWidth, SurvivalSizeBox->DesiredHeight);
	}
	
	if (ProgressBarMain)
	{
		ProgressBarMain->SetWidgetStyle(ProgressBarStyle);
	}

	if (IsDesignTime())
	{
		ProgressBarMain->SetFillColorAndOpacity(StatusBarPreviewFillColor);
	}
}

void USurvivalWidgetBase::SetStatusBarPercent(float InPercent)
{
	if (ProgressBarMain)
	{
		ProgressBarMain->SetPercent(InPercent);
		
		if (bChangeStatusBarFillColorByPercent)
		{
			if (InPercent >= 0.55f)
			{
				ProgressBarMain->SetFillColorAndOpacity(StatusBarDefaultFillColor);
			}
			else if (InPercent <= 0.55f && InPercent >= 0.25f)
			{
				ProgressBarMain->SetFillColorAndOpacity(StatusBarWarningFillColor);
			}
			else
			{
				ProgressBarMain->SetFillColorAndOpacity(StatusBarCriticalFillColor);
			}
		}
		else
		{
			ProgressBarMain->SetFillColorAndOpacity(StatusBarDefaultFillColor);
		}
	}
}
