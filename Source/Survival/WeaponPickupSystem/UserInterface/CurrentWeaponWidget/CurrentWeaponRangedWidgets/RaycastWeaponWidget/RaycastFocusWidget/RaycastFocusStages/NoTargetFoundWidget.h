// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoTargetFoundWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UNoTargetFoundWidget : public UUserWidget
{
	GENERATED_BODY()

public:

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NoTargetFoundText;

	UPROPERTY(meta = (BindWidget))
	UImage* NoTargetFoundImage;
};
