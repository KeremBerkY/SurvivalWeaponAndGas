#pragma once

#include "CoreMinimal.h"
#include "GASAbilityInputID.generated.h"

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Sprint,
	Dash
};