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

void ARocketLauncher::Fire()
{
	Super::Fire();
}


void ARocketLauncher::HandleExplosion(AProjectile* SpawnedProjectile)
{
}

