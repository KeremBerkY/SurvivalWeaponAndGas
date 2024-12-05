// Fill out your copyright notice in the Description page of Project Settings.


#include "FireModeFactory.h"

TScriptInterface<IFireMode> UFireModeFactory::CreateFireMode(EFireMode FireModeType, UObject* Outer)
{
	TScriptInterface<IFireMode> FireModeInstance;

	switch (FireModeType)
	{
	case EFireMode::SingleShot:
		FireModeInstance.SetObject(NewObject<USingleShotFireMode>(Outer));
		break;
	case EFireMode::BurstFire:
		FireModeInstance.SetObject(NewObject<UBurstShotFireMode>(Outer));
		break;
	case EFireMode::Automatic:
		FireModeInstance.SetObject(NewObject<UAutomaticShotFireMode>(Outer));
		break;
	}
		
	if (FireModeInstance.GetObject())
	{
		FireModeInstance.SetInterface(Cast<IFireMode>(FireModeInstance.GetObject()));
		UE_LOG(LogTemp, Warning, TEXT("Created FireMode instance of type %d"), static_cast<int32>(FireModeType));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create FireMode instance of type %d"), static_cast<int32>(FireModeType));
	}
		
	return FireModeInstance;
}
