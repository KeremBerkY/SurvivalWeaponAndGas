// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldPickup.h"

#include "Components/BoxComponent.h"
#include "Survival/SurvivalCharacter.h"


// Sets default values
AGoldPickup::AGoldPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CoinAmount = 50;
	
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
	// CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	// CoinMesh->SetupAttachment(RootComponent);
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AGoldPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoldPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AGoldPickup::Interact(AActor* Actor)
{
	ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(Actor);
	if (Character)
	{
		Destroy();
	}
}

