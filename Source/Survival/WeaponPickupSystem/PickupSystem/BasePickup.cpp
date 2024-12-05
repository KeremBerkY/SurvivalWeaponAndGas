// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"

#include "Components/BoxComponent.h"
#include "Survival/SurvivalCharacter.h"


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

void ABasePickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(OtherActor);
	if (Character)
	{
		// bIsOverlapping = true;
		// Character->CurrentPickup = this;
		Character->CheckNearbyPickups();
	}
}

void ABasePickup::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(OtherActor);
	if (Character)
	{
		// bIsOverlapping = false;
		// Character->CurrentPickup = nullptr;
		Character->CheckNearbyPickups();
	}
}

void ABasePickup::Interact(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("BasePickup class Interact"));
}

void ABasePickup::SetCustomDepth(bool bRenderCustomDepth) const
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(bRenderCustomDepth);
	}
}





