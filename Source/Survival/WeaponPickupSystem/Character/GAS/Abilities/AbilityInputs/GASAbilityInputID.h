#pragma once

#include "CoreMinimal.h"
#include "GASAbilityInputID.generated.h"

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Jump,
	Sprint,
	Dash,
	Interact,
	Fire,
	Reload,
	Drop,
	RaycastAction,
	ProjectileAction,
	MeleeAction,
	ToggleFireMode,
	
};