// Fill out your copyright notice in the Description page of Project Settings.


#include "GetOutFromTargetWidget.h"

#include "Components/RadialSlider.h"

void UGetOutFromTargetWidget::SetRadialSliderValue(const float NewValue) const
{
	if (RadialSlider)
	{
		if (NewValue >= 1.0f)
		{
			RadialSlider->SetValue(0.0f);
		}
		else
		{
			RadialSlider->SetValue(NewValue);
		}
	}
}

void UGetOutFromTargetWidget::ResetRadialSliderValue() const
{
	if (RadialSlider)
	{
		RadialSlider->SetValue(0.f);
	}
}
