// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "GoldPickup.generated.h"

UCLASS()
class SURVIVAL_API AGoldPickup : public ABasePickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGoldPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Actor) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gold")
	int32 CoinAmount;

	// UPROPERTY(EditAnywhere, Category = "Pickup")
	// UStaticMeshComponent* CoinMesh;
};
