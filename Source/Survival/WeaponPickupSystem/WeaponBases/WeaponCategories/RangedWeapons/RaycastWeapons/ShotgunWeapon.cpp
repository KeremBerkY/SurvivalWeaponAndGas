// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Survival/SurvivalCharacter.h"

AShotgunWeapon::AShotgunWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}
void AShotgunWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
void AShotgunWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShotgunWeapon::PerformFire()
{
	Super::PerformFire();

	FVector LocalMuzzleLocation = GetMuzzleLocation()->GetComponentLocation();
	FVector MuzzleForward = GetMuzzleLocation()->GetForwardVector();
	
	// Saçma sayısı ve yayılma açısı
	int32 PelletCount = 10; // Saçma sayısı
	float SpreadAngle = 10.0f; // Maksimum yayılma açısı (derece)
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	OnFirePlayMontage.Broadcast();
	
	for (int32 i = 0; i < PelletCount; i++)
	{
		FVector RandomSpread = FMath::VRandCone(MuzzleForward, FMath::DegreesToRadians(SpreadAngle));

		FVector TraceEnd = LocalMuzzleLocation + (RandomSpread * 5000.0f);

		OnFireMade.Broadcast();
		OnFirePlayMontage.Broadcast();
		
		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(
		   HitResult,
		   LocalMuzzleLocation,
		   TraceEnd,
		   ECC_Visibility,
		   QueryParams
	   );

		if (bHit)
		{
			OnImpact.Broadcast(HitResult);
			
		}

		DrawDebugLine(GetWorld(), LocalMuzzleLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);

	}
}

void AShotgunWeapon::AddFireModes()
{
	Super::AddFireModes();
}


