// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCameraComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterStateComponent.h"

// void UCharacterCameraComponent::SetupCharacterStateListener(ASurvivalCharacter* PlayerCharacter)
// {
// 	if (!PlayerCharacter) return;
//
// 	UCharacterStateComponent* StateComponent = PlayerCharacter->GetCharacterStateComponent();
// 	if (!StateComponent) return;
//
// 	StateComponent->OnCharacterStateChanged.AddDynamic(this, &UCharacterCameraComponent::HandleCharacterStateChanged);
// }
//
// void UCharacterCameraComponent::HandleCharacterStateChanged(ECharacterState NewState)
// {
// }
