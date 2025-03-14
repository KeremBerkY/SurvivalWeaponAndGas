// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalCharacterBase.h"

#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"


ASurvivalCharacterBase::ASurvivalCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	CharacterAbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>("AbilitySystemComponent");
	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
}

UAbilitySystemComponent* ASurvivalCharacterBase::GetAbilitySystemComponent() const
{
	return GetCharacterAbilitySystemComponent();
}

UPawnCombatComponent* ASurvivalCharacterBase::GetPawnCombatComponent() const
{
	return nullptr;
}

void ASurvivalCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivalCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASurvivalCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (CharacterAbilitySystemComponent)
	{
		CharacterAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

