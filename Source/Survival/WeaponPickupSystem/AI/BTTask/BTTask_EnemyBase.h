// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EnemyBase.generated.h"

class ASurvivalEnemyCharacter;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UBTTask_EnemyBase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EnemyBase();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnEnemyExecuteTask(UBehaviorTreeComponent& OwnerComp);
	
	UPROPERTY()
	ASurvivalEnemyCharacter* OwningEnemyCharacter;
};
