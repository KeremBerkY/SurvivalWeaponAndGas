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
	static TMap<EFireMode, TScriptInterface<IFireMode>> CreateFireModes(UObject* Outer, const TArray<EFireMode>& SupportedFireModes);

private:
	static TMap<EFireMode, TScriptInterface<IFireMode>> AddSingleShot(UObject* Outer, TScriptInterface<IFireMode> FireModeInstance, TMap<EFireMode, TScriptInterface<IFireMode>> FireModeMap);
	static TMap<EFireMode, TScriptInterface<IFireMode>> AddBurstShot(UObject* Outer, TScriptInterface<IFireMode> FireModeInstance, TMap<EFireMode, TScriptInterface<IFireMode>> FireModeMap);
	static TMap<EFireMode, TScriptInterface<IFireMode>> AddAutomaticShotMode(UObject* Outer, TScriptInterface<IFireMode> FireModeInstance, TMap<EFireMode, TScriptInterface<IFireMode>> FireModeMap);
};
