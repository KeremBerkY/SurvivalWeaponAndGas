// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetDistToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetDistToTarget::UBTService_GetDistToTarget()
{
	NodeName = TEXT("Native Get Dist To Target");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	InTargetActorKey.AddObjectFilter(this,GET_MEMBER_NAME_CHECKED(ThisClass,InTargetActorKey),AActor::StaticClass());
}

void UBTService_GetDistToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
		OutDistToTargetKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_GetDistToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController ? AIController->GetPawn() : nullptr;

	if (AIPawn)
	{
		if ( UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
		{
			if (AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(InTargetActorKey.SelectedKeyName)))
			{
				float DistanceToTarget = AIPawn->GetDistanceTo(TargetActor);
				BlackboardComp->SetValueAsFloat(OutDistToTargetKey.SelectedKeyName, DistanceToTarget);
			}
		}
	}
	
	
}
