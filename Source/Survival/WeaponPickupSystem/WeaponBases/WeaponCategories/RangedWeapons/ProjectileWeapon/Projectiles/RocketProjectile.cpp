// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketProjectile.h"


ARocketProjectile::ARocketProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// void ARocketProjectile::HandleExplosion()
// {
// 	Super::HandleExplosion();
// 	
// }


