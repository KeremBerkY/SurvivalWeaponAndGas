// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "SurvivalCharacterBase.generated.h"

class UCharacterAttributeSet;
class UCharacterAbilitySystemComponent;

UCLASS()
class SURVIVAL_API ASurvivalCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASurvivalCharacterBase();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponent() const { return CharacterAbilitySystemComponent; }
	FORCEINLINE UCharacterAttributeSet* GetAttributeSet() const { return CharacterAttributeSet; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = true))
	UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = true))
	UCharacterAttributeSet* CharacterAttributeSet;
};
