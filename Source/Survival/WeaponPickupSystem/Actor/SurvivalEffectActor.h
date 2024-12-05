// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurvivalEffectActor.generated.h"

class USphereComponent;
class UGameplayEffect;

UCLASS()
class SURVIVAL_API ASurvivalEffectActor : public AActor
{
	GENERATED_BODY()

public:
	ASurvivalEffectActor();

	// FORCEINLINE TSubclassOf<UGameplayEffect> GetInstantGameplayEffect() const { return InstantGameplayEffect; }

	// void ApplyEffectToTarget(AActor* Target);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// UFUNCTION()
	// virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//
	// UFUNCTION()
	// virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
	
	// UPROPERTY(EditAnywhere, Category = "Applied Effects")
	// TSubclassOf<UGameplayEffect> InstantGameplayEffect;

	// UPROPERTY(EditAnywhere)
	// TObjectPtr<USphereComponent> Sphere;
	//
	// UPROPERTY(EditAnywhere)
	// TObjectPtr<UStaticMeshComponent> Mesh;

};
