// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateComponent.h"

#include "CharacterTypes.h"


UCharacterStateComponent::UCharacterStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentCharacterState = ECharacterState::Idle; 
}


void UCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}


void UCharacterStateComponent::UpdateCharacterState(ECharacterState NewState)
{
	if (CurrentCharacterState == NewState) return;
	
	CurrentCharacterState = NewState;
	
	// OnCharacterStateChanged.Broadcast(NewState);
}
