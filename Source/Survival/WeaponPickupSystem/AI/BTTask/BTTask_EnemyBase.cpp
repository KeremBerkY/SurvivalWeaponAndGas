// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyBase.h"

#include "AIController.h"
#include "Survival/WeaponPickupSystem/CharacterBase/Enemy/SurvivalEnemyCharacter.h"

UBTTask_EnemyBase::UBTTask_EnemyBase()
{
	NodeName = TEXT("Native Enemy Base");
}

EBTNodeResult::Type UBTTask_EnemyBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (OwningEnemyCharacter)
	{
		if (ControlledPawn && AIController && BlackboardComp)
		{
			OnEnemyExecuteTask(OwnerComp);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::InProgress;
		}
	}
	else
	{
		OwningEnemyCharacter = Cast<ASurvivalEnemyCharacter>(ControlledPawn);
		if (OwningEnemyCharacter)
		{
			OnEnemyExecuteTask(OwnerComp);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::InProgress;
		}
		else
		{
			FinishLatentTask(OwnerComp,  EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
	}
	
	return EBTNodeResult::InProgress;

}

void UBTTask_EnemyBase::OnEnemyExecuteTask(UBehaviorTreeComponent& OwnerComp)
{
}
