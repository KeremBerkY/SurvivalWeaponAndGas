// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"


ASurvivalEffectActor::ASurvivalEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	// SetRootComponent(Mesh);
	//
	// Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	// Sphere->SetupAttachment(GetRootComponent());
}

void ASurvivalEffectActor::BeginPlay()
{
	Super::BeginPlay();

	// Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASurvivalEffectActor::OnOverlap);
	// Sphere->OnComponentEndOverlap.AddDynamic(this, &ASurvivalEffectActor::EndOverlap);
}

void ASurvivalEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// void ASurvivalEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	ApplyEffectToTarget(OtherActor, InstantGameplayEffect);
// 	// K2_DestroyActor();
// }
//
// void ASurvivalEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
// {
// 	
// }

// void ASurvivalEffectActor::ApplyEffectToTarget(AActor* Target)
// {
// 	ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(Target);
// 	UCharacterAbilitySystemComponent* TargetASC = PlayerCharacter->GetCharacterAbilitySystemComponent();
// 	if (TargetASC == nullptr) return;
//
// 	check(GetInstantGameplayEffect());
// 	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
// 	EffectContextHandle.AddSourceObject(this);
//
// 	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GetInstantGameplayEffect(), 1, EffectContextHandle);
// 	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
// }

