// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "Components/ActorComponent.h"
#include "CharacterStateComponent.generated.h"

/*
 *		BU SINIF KULLANILMIYOR TAGLAR İLE YÖNETİCEZ
 */

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStateChanged, ECharacterState, NewState);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UCharacterStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStateComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;


	/* DELEGATES */
	// UPROPERTY(BlueprintAssignable, Category = "Events")
	// FOnCharacterStateChanged OnCharacterStateChanged;

							/* Getters/Setters */
	FORCEINLINE ECharacterState GetCurrentState() const { return CurrentCharacterState; }
	FORCEINLINE void SetCharacterState(ECharacterState NewCharacterState) { CurrentCharacterState = NewCharacterState; }

	void UpdateCharacterState(ECharacterState NewState);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ECharacterState CurrentCharacterState;

};
