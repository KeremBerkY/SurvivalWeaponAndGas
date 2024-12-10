// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutomaticShotFireMode.h"
#include "BurstShotFireMode.h"
#include "FireMode.h"
#include "FireModeConfig.h"
#include "SingleShotFireMode.h"
#include "UObject/Object.h"
#include "FireModeFactory.generated.h"


/**
 * 
 */
UCLASS()
class SURVIVAL_API UFireModeFactory : public UObject
{
	GENERATED_BODY()

public:
	static TScriptInterface<IFireMode> CreateFireMode(EFireMode FireModeType, UObject* Outer);
	
};
