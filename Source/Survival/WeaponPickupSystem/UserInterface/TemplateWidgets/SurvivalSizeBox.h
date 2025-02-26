// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SizeBox.h"
#include "SurvivalSizeBox.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API USurvivalSizeBox : public USizeBox
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	float DesiredWidth = 210.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	float DesiredHeight = 210.0f;
	
	virtual void SynchronizeProperties() override;

	virtual void SetWidthHeightOverride(float SizeBoxWidthOverride, float SizeBoxHeightOverride);
};
