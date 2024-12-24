// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"

#include "Components/BoxComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/PickupComponent.h"


// Sets default values
ABasePickup::ABasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = Mesh;
	
	PickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupCollision"));
	PickupCollision->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	PickupCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PickupCollision->SetGenerateOverlapEvents(true);
    
	PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::OnPickupOverlapBegin);
	PickupCollision->OnComponentEndOverlap.AddDynamic(this, &ABasePickup::OnPickupOverlapEnd);
	PickupCollision->SetupAttachment(GetRootComponent());

	bIsOverlapping = false;
}

void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasePickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(OtherActor))
	{
		if (UPickupComponent* PickupComponent = PlayerCharacter->GetPickupComponent())
		{
			PickupComponent->CheckNearbyPickups();
		}
	}
}

void ABasePickup::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(OtherActor))
	{
		if (UPickupComponent* PickupComponent = PlayerCharacter->GetPickupComponent())
		{
			PickupComponent->CheckNearbyPickups();
		}
	}
}

void ABasePickup::Interact(AActor* Actor)
{
}

void ABasePickup::SetCustomDepth(bool bRenderCustomDepth) const
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(bRenderCustomDepth);
		Mesh->SetCustomDepthStencilValue(1);
	}
}





