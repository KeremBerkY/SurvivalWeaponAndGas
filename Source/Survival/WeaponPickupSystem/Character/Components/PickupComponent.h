// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/WeaponPickupSystem/PickupSystem/BasePickup.h"
#include "PickupComponent.generated.h"

class USphereComponent;
class ASurvivalSystemHUD;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickupUpdated, const FInteractableData&, InteractableData); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowWidget); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideWidget); 



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPickupComponent();

	// DELEGATE
	FOnPickupUpdated OnPickupUpdated;
	FOnShowWidget ShowWidget;
	FOnHideWidget HideWidget;

	FORCEINLINE ABasePickup* GetCurrentPickup() const { return CurrentPickup; }
	FORCEINLINE void SetCurrentPickup(ABasePickup* NewPickup) { CurrentPickup = NewPickup; }

	void CheckNearbyPickups();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void CheckPickupInterval(float DeltaTime);
	void UpdateCurrentPickup(ABasePickup* NewPickup);

	void BindPickupCallbacks();

private:
	UPROPERTY()
	ABasePickup* CurrentPickup;
	
};
