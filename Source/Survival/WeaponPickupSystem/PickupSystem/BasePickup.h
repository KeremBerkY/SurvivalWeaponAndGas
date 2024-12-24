// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Survival/WeaponPickupSystem/Interfaces/InteractionInterface.h"
#include "BasePickup.generated.h"

class UBoxComponent;

// TODO: CheckPickupInterval içerisindeyse Pickuplar havaya doğru kalksın ve olduğu yerde dönmeye başlasın. Eğer silah ise ekstra olarak özellikleri USerWidget ile ekranda bir kutu içreisinde yazsın.


UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"), // Gold (for now)
	Weapon UMETA(DisplayName = "Weapon"), // Raycast, Projectile & Melee Weapons
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
	Device UMETA(DisplayName = "Device"), // door, gate, window
	Toggle UMETA(DisplayName = "Toggle"), // button, switch, activate some functionality
	Container UMETA(DisplayName = "Container"),
};

USTRUCT(BlueprintType)
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() : InteractableType(EInteractableType::Pickup), Name(FText::GetEmpty()),
			Action(FText::GetEmpty()), Quantity(0), InteractionDuration(0.f) {};

	
	UPROPERTY(EditAnywhere, Category = "Interactable Data")
	EInteractableType InteractableType;
	
	UPROPERTY(EditAnywhere, Category = "Interactable Data")
	FText Name;
	
	UPROPERTY(EditAnywhere, Category = "Interactable Data")
	FText Action;
	
	UPROPERTY(EditAnywhere, Category = "Interactable Data")
	int32 Quantity; // Used only for pickups
	
	UPROPERTY(EditAnywhere, Category = "Interactable Data")
	float InteractionDuration; // Used for things like valves, doors, etc. that require an interaction timer
};

UCLASS()
class SURVIVAL_API ABasePickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasePickup();

	virtual void Interact(AActor* Actor) override;
	void SetCustomDepth(bool bRenderCustomDepth) const;
	
	bool bIsOverlapping;

	UPROPERTY(EditAnywhere, Category = "Pickup | Interactable Data")
	FInteractableData InteractableData;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	UBoxComponent* PickupCollision;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	UStaticMeshComponent* Mesh;
	
};
