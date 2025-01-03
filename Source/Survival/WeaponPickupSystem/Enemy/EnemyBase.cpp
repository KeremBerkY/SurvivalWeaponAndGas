// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "EnemyComponents/LockedWidgetComponent.h"
#include "EnemyComponents/SelectedWidgetComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	LockedWidgetComponent = CreateDefaultSubobject<ULockedWidgetComponent>(TEXT("LockedWidgetComponent"));
	LockedWidgetComponent->SetupAttachment(RootComponent);

	SelectedWidgetComponent = CreateDefaultSubobject<USelectedWidgetComponent>(TEXT("SelectedWidgetCOmponent"));
	SelectedWidgetComponent->SetupAttachment(RootComponent);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
