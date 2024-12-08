// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/PickupSystem/BasePickup.h"
#include "Survival/WeaponPickupSystem/PickupSystem/Interfaces/InteractionInterface.h"


void UInteractAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Interact Called."));

	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (ABasePickup* CurrentPickup = PlayerCharacter->GetCurrentPickup())
		{
			if (IInteractionInterface* Interactable = Cast<IInteractionInterface>(CurrentPickup))
			{
				Interactable->Interact(PlayerCharacter);
				PlayerCharacter->SetCurrentPickup(nullptr);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No pickup available to interact with."));
		}
	}

	// Current haline getirdim bir hata alırsan bundan olabilir
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UInteractAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
