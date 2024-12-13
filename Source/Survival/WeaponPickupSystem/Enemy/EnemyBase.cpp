// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
