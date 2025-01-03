// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockonUI.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class SURVIVAL_API ULockonUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "LockonWidget")
	UImage* LockImage;
	
};
