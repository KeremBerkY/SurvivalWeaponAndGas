// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomDepthHelper.h"

void UCustomDepthHelper::SetCustomDepth(UPrimitiveComponent* Mesh, bool bRenderCustomDepth, int32 StencilValue)
{
	if (!Mesh)
	{
		return;
	}
	
	if (bRenderCustomDepth)
	{
		Mesh->SetCustomDepthStencilValue(StencilValue);
		Mesh->SetRenderCustomDepth(bRenderCustomDepth);	
		UE_LOG(LogTemp, Log, TEXT("Custom Depth Enabled: StencilValue=%d"), StencilValue);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Custom Depth Disabled"));
	}
}

void UCustomDepthHelper::ResetCustomDepth(UPrimitiveComponent* Mesh)
{
	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResetCustomDepth: Mesh is null!"));
		return;
	}

	Mesh->SetCustomDepthStencilValue(0);
	Mesh->SetRenderCustomDepth(false);
	UE_LOG(LogTemp, Log, TEXT("Custom Depth Reset"));
}
