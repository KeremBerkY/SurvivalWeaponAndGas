// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketLauncher.h"


ARocketLauncher::ARocketLauncher()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARocketLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARocketLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARocketLauncher::Attack()
{
	Super::Attack();

	
}

void ARocketLauncher::EndAttack()
{
	Super::EndAttack();
}


// void ARocketLauncher::HandleExplosion(AProjectile* SpawnedProjectile)
// {
// }

