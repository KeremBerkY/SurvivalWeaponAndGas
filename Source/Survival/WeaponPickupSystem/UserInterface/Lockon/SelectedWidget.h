// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectedWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class SURVIVAL_API USelectedWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "LockonWidget")
	UImage* SelectImage;
};
