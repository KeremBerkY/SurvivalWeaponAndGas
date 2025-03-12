// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_ComputeSuccessChance.h"

#include "Kismet/KismetMathLibrary.h"

UBTDecorator_ComputeSuccessChance::UBTDecorator_ComputeSuccessChance()
{
	NodeName = "Do I want to attack?";
}

bool UBTDecorator_ComputeSuccessChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                   uint8* NodeMemory) const
{
	float RandomWeight = FMath::RandRange(SuccessChanceMin, SuccessChanceMax);
	
	return UKismetMathLibrary::RandomBoolWithWeight(RandomWeight);
}
