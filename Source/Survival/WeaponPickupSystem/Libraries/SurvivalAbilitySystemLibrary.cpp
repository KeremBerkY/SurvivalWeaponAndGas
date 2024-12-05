// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/SurvivalGameMode.h"

UCharacterClassInfo* USurvivalAbilitySystemLibrary::GetCharacterClassDefaultInfo(const UObject* WorldContextObject)
{
	if (const ASurvivalGameMode* SurvivalGameMode = Cast<ASurvivalGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return SurvivalGameMode->GetCharacterClassDefaultInfo();
	}
	
	return nullptr;
}

UCharacterAbilitySystemComponent* USurvivalAbilitySystemLibrary::GetCharacterAbilitySystemComponent(const AActor* Actor)
{
	
		if (!Actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("GetCharacterAbilitySystemComponent: Actor is null!"));
			return nullptr;
		}

		// Check if Actor is a SurvivalCharacter
		if (!Actor->IsA(ASurvivalCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GetCharacterAbilitySystemComponent: Actor is not a SurvivalCharacter."));
			return nullptr;
		}

		const ACharacter* Character = Cast<ACharacter>(Actor);
		if (!Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("GetCharacterAbilitySystemComponent: Failed to cast Actor to ACharacter."));
			return nullptr;
		}

		// Find the Ability System Component
		UActorComponent* Component = Character->GetComponentByClass(UCharacterAbilitySystemComponent::StaticClass());
		if (!Component)
		{
			UE_LOG(LogTemp, Warning, TEXT("GetCharacterAbilitySystemComponent: AbilitySystemComponent not found."));
			return nullptr;
		}

		// Cast to UCharacterAbilitySystemComponent
		UCharacterAbilitySystemComponent* AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(Component);
		if (!AbilitySystemComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("GetCharacterAbilitySystemComponent: Found component is not a UCharacterAbilitySystemComponent."));
			return nullptr;
		}

		return AbilitySystemComponent;
	

	// if (!Actor)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("GetCharacterAbilitySystemComponent: Actor is null!"));
	// 	return nullptr;
	// }
	//
	// // Oyuncunun AbilitySystemComponent'ini al
	// if (const ACharacter* Character = Cast<ASurvivalCharacter>(Actor))
	// {
	// 	return Cast<UCharacterAbilitySystemComponent>(Character->GetComponentByClass(UCharacterAbilitySystemComponent::StaticClass()));
	// }
	//
	// UE_LOG(LogTemp, Warning, TEXT("GetCharacterAbilitySystemComponent: Actor is not a Character or does not have UCharacterAbilitySystemComponent."));
	// return nullptr;
}
