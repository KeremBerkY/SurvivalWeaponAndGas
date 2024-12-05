// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourcePickup.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Actor/SurvivalEffectActor.h"
#include "Survival/WeaponPickupSystem/Character/PlayerStates/CharacterPlayerState.h"
#include "Survival/WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"


AResourcePickup::AResourcePickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AResourcePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AResourcePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AResourcePickup::Interact(AActor* Actor)
{
	Super::Interact(Actor);

	ApplyEffectToTarget(Actor);
	
}

void AResourcePickup::ApplyEffectToTarget(AActor* Target)
{
	ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(Target);
	UCharacterAbilitySystemComponent* TargetASC = PlayerCharacter->GetCharacterAbilitySystemComponent();
	if (TargetASC == nullptr) return;

	check(GetInstantGameplayEffect());
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GetInstantGameplayEffect(), 1, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

