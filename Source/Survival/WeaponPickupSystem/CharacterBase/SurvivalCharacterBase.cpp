// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalCharacterBase.h"

#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"


ASurvivalCharacterBase::ASurvivalCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterAbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>("AbilitySystemComponent");
	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
}

UAbilitySystemComponent* ASurvivalCharacterBase::GetAbilitySystemComponent() const
{
	return GetCharacterAbilitySystemComponent();
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

