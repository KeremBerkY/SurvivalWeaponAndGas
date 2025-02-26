// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateTypes.h"
#include "Components/ProgressBar.h"
#include "SurvivalWidgetBase.generated.h"


class USurvivalSizeBox;
class UProgressBar;
class USizeBox;
/**
 * 
 */
UCLASS()
class SURVIVAL_API USurvivalWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// virtual void NativePreConstruct() override;
	virtual void SynchronizeProperties() override;


	void SetStatusBarPercent(float InPercent);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (BindWidget))
	USurvivalSizeBox* SurvivalSizeBox;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBarMain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FProgressBarStyle ProgressBarStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style | Color")
	FLinearColor StatusBarPreviewFillColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style | Color")
	FLinearColor StatusBarDefaultFillColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style | Color")
	FLinearColor StatusBarWarningFillColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style | Color")
	FLinearColor StatusBarCriticalFillColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style | Color")
	bool bChangeStatusBarFillColorByPercent = true;
};
