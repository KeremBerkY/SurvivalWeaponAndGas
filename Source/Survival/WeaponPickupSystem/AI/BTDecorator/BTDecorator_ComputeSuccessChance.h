// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ComputeSuccessChance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UBTDecorator_ComputeSuccessChance : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_ComputeSuccessChance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SuccuessChance", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0" ))
	float SuccessChanceMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SuccuessChance", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0" ))
	float SuccessChanceMax;
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
