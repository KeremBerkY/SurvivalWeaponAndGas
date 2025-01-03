// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"

class AEnemyBase;
class UFocusCrosshair;
class UCharacterMovementComponent;
class ASurvivalCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULockonComponent();

	FORCEINLINE bool IsLocked() const { return bIsLocked; }
	FORCEINLINE void SetLocked(bool IsLocked) { bIsLocked = IsLocked; }
	FORCEINLINE AEnemyBase* GetCurrentTargetActor() const { return CurrentTargetActor; }
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
	UPROPERTY()
	UFocusCrosshair* FocusCrosshair;
	
	TWeakObjectPtr<ASurvivalCharacter> PlayerCharacterPtr;

	TWeakObjectPtr<APlayerController> ControllerPtr;

	TWeakObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY()
	AEnemyBase* CurrentTargetActor;

	UPROPERTY()
	AEnemyBase* SelectedActor;

	bool bIsLocked;

};
