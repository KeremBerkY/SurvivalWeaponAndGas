// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalSizeBox.h"

void USurvivalSizeBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	SetWidthHeightOverride(DesiredWidth, DesiredHeight);
}

void USurvivalSizeBox::SetWidthHeightOverride(float SizeBoxWidthOverride, float SizeBoxHeightOverride)
{
	SetWidthOverride(SizeBoxWidthOverride);
	SetHeightOverride(SizeBoxHeightOverride);
}
