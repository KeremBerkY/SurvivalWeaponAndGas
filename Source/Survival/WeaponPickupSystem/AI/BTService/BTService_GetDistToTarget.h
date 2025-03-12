// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetDistToTarget.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UBTService_GetDistToTarget : public UBTService
{
	GENERATED_BODY()
	
	UBTService_GetDistToTarget();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	//~ End UBTNode Interface

	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector OutDistToTargetKey;
};
