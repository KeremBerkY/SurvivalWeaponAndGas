// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FocusCrosshair.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UFocusCrosshair : public UUserWidget
{
	GENERATED_BODY()

public:

	void ShowFocusCrosshair();
	void HideFocusCrosshair();
	
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "LockonWidget")
	UImage* FocusCrosshair;
};
