// Fill out your copyright notice in the Description page of Project Settings.


#include "FocusCrosshair.h"

#include "Components/Image.h"

void UFocusCrosshair::ShowFocusCrosshair()
		{
	if (FocusCrosshair)
	{
		FocusCrosshair->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFocusCrosshair::HideFocusCrosshair()
{
	if (FocusCrosshair)
	{
		FocusCrosshair->SetVisibility(ESlateVisibility::Hidden);
	}
}
