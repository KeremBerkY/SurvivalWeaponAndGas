// Fill out your copyright notice in the Description page of Project Settings.


#include "FireModeFactory.h"
#include "Survival/WeaponPickupSystem/WeaponBases/FireModes/FireMode.h"

TMap<EFireMode, TScriptInterface<IFireMode>> UFireModeFactory::CreateFireModes(UObject* Outer, const TArray<EFireMode>& SupportedFireModes)
{
	TMap<EFireMode, TScriptInterface<IFireMode>> FireModeMap;
	
	for (const EFireMode& FireModeType : SupportedFireModes)
	{
		TScriptInterface<IFireMode> FireModeInstance;

		switch (FireModeType) {
		case EFireMode::SingleShot:
			{
				FireModeMap = AddSingleShot(Outer, FireModeInstance, FireModeMap);
			}
			break;
		case EFireMode::BurstFire:
			{
				FireModeMap = AddBurstShot(Outer, FireModeInstance, FireModeMap);
			break;
			}
		case EFireMode::Automatic:
			{
				FireModeMap = AddAutomaticShotMode(Outer, FireModeInstance, FireModeMap);
			}
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Unsupported Fire Mode: %d"), static_cast<int32>(FireModeType));
			break;
		}
	}
	
	return FireModeMap;
}

TMap<EFireMode, TScriptInterface<IFireMode>> UFireModeFactory::AddSingleShot(UObject* Outer, TScriptInterface<IFireMode> FireModeInstance, TMap<EFireMode, TScriptInterface<IFireMode>> FireModeMap)
{
	UObject* SingleShotMode = NewObject<USingleShotFireMode>(Outer);
	
	FireModeInstance.SetObject(SingleShotMode);
	FireModeInstance.SetInterface(Cast<IFireMode>(SingleShotMode));
	FireModeMap.Add(EFireMode::SingleShot, FireModeInstance);
	
	return FireModeMap;
}

TMap<EFireMode, TScriptInterface<IFireMode>> UFireModeFactory::AddBurstShot(UObject* Outer, TScriptInterface<IFireMode> FireModeInstance, TMap<EFireMode, TScriptInterface<IFireMode>> FireModeMap)
{
	UObject* BurstFireMode = NewObject<UBurstShotFireMode>(Outer);
	
	FireModeInstance.SetObject(BurstFireMode);
	FireModeInstance.SetInterface(Cast<IFireMode>(BurstFireMode));
	FireModeMap.Add(EFireMode::BurstFire, FireModeInstance);
	
	return FireModeMap;
}

TMap<EFireMode, TScriptInterface<IFireMode>> UFireModeFactory::AddAutomaticShotMode(UObject* Outer, TScriptInterface<IFireMode> FireModeInstance, TMap<EFireMode, TScriptInterface<IFireMode>> FireModeMap)
{
	UObject* AutomaticMode = NewObject<UAutomaticShotFireMode>(Outer);
	
	FireModeInstance.SetObject(AutomaticMode);
	FireModeInstance.SetInterface(Cast<IFireMode>(AutomaticMode));
	FireModeMap.Add(EFireMode::Automatic, FireModeInstance);
	
	return FireModeMap;
}

