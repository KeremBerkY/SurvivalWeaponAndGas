// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"

class ASurvivalEnemyCharacter;
enum class ETargetWidgetState : uint8;
class AEnemyBase;
class UFocusCrosshair;
class UCharacterMovementComponent;
class ASurvivalCharacter;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetFound, ETargetWidgetState, NewState);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	
	ULockonComponent();

	FORCEINLINE bool IsLocked() const { return bIsLocked; }
	FORCEINLINE void SetLocked(bool IsLocked) { bIsLocked = IsLocked; }
	// FORCEINLINE AEnemyBase* GetCurrentTargetActor() const { return CurrentTargetActor; }
	FORCEINLINE ASurvivalEnemyCharacter* GetCurrentTargetActor() const { return CurrentTargetActor; }
	FORCEINLINE UFocusCrosshair* GetFocusCrosshair() const { return FocusCrosshair; }

	void RemoveFocusCrosshair() const;
	void AddFocusCrosshair() const;

	void StartLockon();
	void EndLockon();

	void PerformSelect();
	void EndSelect() const;
	
	void DrawDebugVisuals(const FVector& Start, const FVector& End, const FHitResult& HitResult) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UFocusCrosshair> FocusCrosshairClass;
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FocusCrosshairInitialize();

private:
	void CheckCurrentWeaponAndCategory();
	void CheckAndPerformTargetSelection(float DeltaTime);
	void UpdateTargetState(ETargetWidgetState NewState) const;
	void RotateTowardsTarget(float DeltaTime) const;
	
	UPROPERTY()
	UFocusCrosshair* FocusCrosshair;
	
	TWeakObjectPtr<ASurvivalCharacter> PlayerCharacterPtr;

	TWeakObjectPtr<APlayerController> ControllerPtr;

	TWeakObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY()
	// AEnemyBase* CurrentTargetActor;
	ASurvivalEnemyCharacter* CurrentTargetActor;

	UPROPERTY()
	// AEnemyBase* SelectedActor;
	ASurvivalEnemyCharacter* SelectedActor;

	bool bIsLocked;

	bool bIsNotRaycast;
};
