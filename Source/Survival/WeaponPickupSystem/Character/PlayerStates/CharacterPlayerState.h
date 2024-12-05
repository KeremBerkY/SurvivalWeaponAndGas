// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CharacterPlayerState.generated.h"

class UCharacterAttributeSet;
class UCharacterAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class SURVIVAL_API ACharacterPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure)
	UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponent() const;
	
	UFUNCTION(BlueprintPure)
	UCharacterAttributeSet* GetCharacterAttributes() const;

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterAbilitySystemComponent> CharacterAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterAttributeSet> CharacterAttributes;
};
