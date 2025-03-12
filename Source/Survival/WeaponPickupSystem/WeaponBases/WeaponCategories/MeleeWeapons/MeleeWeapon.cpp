// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "Components/BoxComponent.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/MeleeWeaponUIHandler/MeleeWeaponUIHandler.h"


AMeleeWeapon::AMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionBoxEndOverlap);

	MeleeWeaponUIHandler = CreateDefaultSubobject<UMeleeWeaponUIHandler>(TEXT("MeleeWeaponUIHandler"));
}

void AMeleeWeapon::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn for the weapon: %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (USurvivalAbilitySystemLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			OnWeaponHitTarget.Broadcast(OtherActor);
		}
	}
	
	// if (const APawn* HitPawn = Cast<APawn>(OtherActor))
	// {
	// 	if (WeaponOwningPawn != HitPawn)
	// 	{
	// 		OnWeaponHitTarget.Broadcast(OtherActor);
	// 	}
	//
	// 	// TODO: Implement hit check for enemy characters
	// }
}

void AMeleeWeapon::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the onwning pawn for the weapon: %s"), *GetName());
	
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (USurvivalAbilitySystemLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			OnWeaponPulledFromTarget.Broadcast(OtherActor);
		}
	}
	
	// if (const APawn* HitPawn = Cast<APawn>(OtherActor))
	// {
	// 	if (WeaponOwningPawn != HitPawn)
	// 	{
	// 		OnWeaponPulledFromTarget.Broadcast(OtherActor);
	// 	}
	//
	// 	// TODO: Implement hit check for enemy characters
	// }
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

