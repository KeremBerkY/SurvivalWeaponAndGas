// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CharacterStateManager.generated.h"

class UAbilitySystemComponent;

/*  Yalnızca durum ve eylem yönetimiyle ilgileniyor; bu, Single Responsibility Principle (Tek Sorumluluk Prensibi)'ne uygundur. */
// TODO: OnStateAdded & OnStateRemoved aynısı action için yaparak daha esnek yapabiliriz. Ama gerek var mı?

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, FGameplayTag, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionChanged, FGameplayTag, NewAction);

UCLASS(Blueprintable)
class SURVIVAL_API UCharacterStateManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UAbilitySystemComponent* InAbilitySystemComponent);
	
	// State Yönetimi
	UFUNCTION(BlueprintCallable, Category = "State Manager")
	void AddState(FGameplayTag StateTag);
	
	UFUNCTION(BlueprintCallable, Category = "State Manager")
	void RemoveState(FGameplayTag StateTag);
	
	UFUNCTION(BlueprintCallable, Category = "State Manager")
	bool HasState(FGameplayTag StateTag) const;

	UFUNCTION(BlueprintCallable, Category = "State Manager")
	bool TransitionToState(FGameplayTag FromState, FGameplayTag ToState);
	
	// Action Yönetimi
	UFUNCTION(BlueprintCallable, Category = "State Manager")
	void AddAction(FGameplayTag ActionTag);
	
	UFUNCTION(BlueprintCallable, Category = "State Manager")
	void RemoveAction(FGameplayTag ActionTag);
	
	UFUNCTION(BlueprintCallable, Category = "State Manager")
	bool HasAction(FGameplayTag ActionTag) const;

	UFUNCTION(BlueprintCallable, Category = "State Manager")
	bool TransitionToAction(FGameplayTag FromAction, FGameplayTag ToAction);
	
protected:
	
	// Event Dispatchers
	UPROPERTY(BlueprintAssignable, Category = "State Manager")
	FOnStateChanged OnStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "State Manager")
	FOnActionChanged OnActionChanged;
	
private:
	UAbilitySystemComponent* AbilitySystemComponent;
	
};
