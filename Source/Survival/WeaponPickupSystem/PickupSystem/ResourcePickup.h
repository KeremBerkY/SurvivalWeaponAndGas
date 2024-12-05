// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "ResourcePickup.generated.h"

class UGameplayEffect;
class ASurvivalEffectActor;

UCLASS()
class SURVIVAL_API AResourcePickup : public ABasePickup
{
	GENERATED_BODY()

public:
	AResourcePickup();

	FORCEINLINE TSubclassOf<UGameplayEffect> GetInstantGameplayEffect() const { return InstantGameplayEffect; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AActor* Actor) override;
	void ApplyEffectToTarget(AActor* Target);

private:

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffect;

};
