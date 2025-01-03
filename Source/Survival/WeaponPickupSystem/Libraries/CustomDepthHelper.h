// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomDepthHelper.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UCustomDepthHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Custom Depth")
	static void SetCustomDepth(UPrimitiveComponent* Mesh, bool bRenderCustomDepth, int32 StencilValue);

	UFUNCTION(BlueprintCallable, Category = "Custom Depth")
	static void ResetCustomDepth(UPrimitiveComponent* Mesh);
};
