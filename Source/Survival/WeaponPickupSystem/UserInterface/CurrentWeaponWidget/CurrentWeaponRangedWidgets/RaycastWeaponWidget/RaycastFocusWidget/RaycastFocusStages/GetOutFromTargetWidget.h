// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GetOutFromTargetWidget.generated.h"

class URadialSlider;
class UTextBlock;
class URadialSliderWidget;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UGetOutFromTargetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetRadialSliderValue(float NewValue) const;
	void ResetRadialSliderValue() const;
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GetOutFromTargetText;

	UPROPERTY(meta = (BindWidget))
	URadialSlider* RadialSlider;
};
