// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Fire.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFire : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURVIVAL_API IFire
{
	GENERATED_BODY()

public:
	virtual void Fire();
	virtual void EndFire();
};
