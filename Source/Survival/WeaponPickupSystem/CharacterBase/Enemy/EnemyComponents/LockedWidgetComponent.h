// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LockedWidgetComponent.generated.h"


class ULockonUI;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API ULockedWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	ULockedWidgetComponent();

	void ShowLockWidget();
	void HideLockWidget();

};
