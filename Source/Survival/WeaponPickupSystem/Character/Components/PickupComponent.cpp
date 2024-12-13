// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"

#include "Components/SphereComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/PickupSystem/BasePickup.h"

UPickupComponent::UPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("Pickup Component initialize..."));
}

void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();
	
	BindPickupCallbacks();
}

void UPickupComponent::BindPickupCallbacks()
{
	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetOwner()))
	{
		PlayerCharacter->GetPickupSphere()->OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::OnPickupOverlapBegin);
		PlayerCharacter->GetPickupSphere()->OnComponentEndOverlap.AddDynamic(this, &UPickupComponent::OnPickupOverlapEnd);
	}
}

void UPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckPickupInterval(DeltaTime);
}


void UPickupComponent::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABasePickup* Pickup = Cast<ABasePickup>(OtherActor))
	{
		UpdateCurrentPickup(Pickup);
	}
}

void UPickupComponent::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABasePickup* Pickup = Cast<ABasePickup>(OtherActor); Pickup && Pickup == GetCurrentPickup())
	{
		UpdateCurrentPickup(nullptr);
	}
}

void UPickupComponent::UpdateCurrentPickup(ABasePickup* NewPickup)
{
	if (GetCurrentPickup())
	{
		CurrentPickup->SetCustomDepth(false);
	}

	CurrentPickup = NewPickup;
	SetCurrentPickup(NewPickup);
	
	if (CurrentPickup)
	{
		CurrentPickup->SetCustomDepth(true);
		
		OnPickupUpdated.Broadcast(CurrentPickup->InteractableData);
		ShowWidget.Broadcast();
	}
	else
	{
		HideWidget.Broadcast();
	}
}

void UPickupComponent::CheckPickupInterval(float DeltaTime)
{
	static float TimeAccumulator = 0.f;
	TimeAccumulator += DeltaTime;
	if (TimeAccumulator >= 0.5f)
	{
		CheckNearbyPickups();
		TimeAccumulator = 0.f;
	}
}

void UPickupComponent::CheckNearbyPickups()
{
	ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetOwner());
	if (!PlayerCharacter) return;
	
	TArray<AActor*> OverlappingActors;
	PlayerCharacter->GetPickupSphere()->GetOverlappingActors(OverlappingActors, ABasePickup::StaticClass());

	float NearestDistance = TNumericLimits<float>::Max();
	ABasePickup* NearestPickup = nullptr;

	
	for (AActor* Actor : OverlappingActors)
	{
		ABasePickup* Pickup = Cast<ABasePickup>(Actor);
		if (Pickup)
		{
			float Distance = (Pickup->GetActorLocation() - PlayerCharacter->GetActorLocation()).Size();
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestPickup = Pickup;
			}
		}
	}
	
}
