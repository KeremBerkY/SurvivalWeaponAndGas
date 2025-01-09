// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetFoundWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UTargetFoundWidget : public UUserWidget
{
	GENERATED_BODY()

public:

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TargetFoundText;

	UPROPERTY(meta = (BindWidget))
	UImage* TargetFoundImage;
};
