// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateManager.h"
#include "AbilitySystemComponent.h"


void UCharacterStateManager::Initialize(UAbilitySystemComponent* InAbilitySystemComponent)
{
	AbilitySystemComponent = InAbilitySystemComponent;
}

void UCharacterStateManager::AddState(FGameplayTag StateTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AddLooseGameplayTag(StateTag);
	}
}

void UCharacterStateManager::RemoveState(FGameplayTag StateTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(StateTag);
	}
}

bool UCharacterStateManager::HasState(FGameplayTag StateTag) const
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->HasMatchingGameplayTag(StateTag);
	}
	return false;
}

bool UCharacterStateManager::TransitionToState(FGameplayTag FromState, FGameplayTag ToState)
{
	if (!HasState(FromState))
	{
		UE_LOG(LogTemp, Warning, TEXT("TransitionToState: Karakter şu anda %s durumunda değil!"), *FromState.ToString());
		return false;
	}

	RemoveState(FromState);
	AddState(ToState);
	
	OnStateChanged.Broadcast(ToState);

	UE_LOG(LogTemp, Log, TEXT("TransitionToState: %s durumundan %s durumuna geçiş yapıldı."), *FromState.ToString(), *ToState.ToString());
	return true;
}

void UCharacterStateManager::AddAction(FGameplayTag ActionTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AddLooseGameplayTag(ActionTag);
	}
}

void UCharacterStateManager::RemoveAction(FGameplayTag ActionTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(ActionTag);
	}
}

bool UCharacterStateManager::HasAction(FGameplayTag ActionTag) const
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->HasMatchingGameplayTag(ActionTag);
	}
	return false;
}

bool UCharacterStateManager::TransitionToAction(FGameplayTag FromAction, FGameplayTag ToAction)
{
	if (!HasAction(FromAction))
	{
		UE_LOG(LogTemp, Warning, TEXT("TransitionToAction: Karakter şu anda %s aksiyonunda değil!"), *FromAction.ToString());
		return false;
	}
	
	RemoveAction(FromAction);
	AddAction(ToAction);

	OnActionChanged.Broadcast(ToAction);

	UE_LOG(LogTemp, Log, TEXT("TransitionToAction: %s aksiyonundan %s aksiyonuna geçiş yapıldı."), *FromAction.ToString(), *ToAction.ToString()); // Saldırırken birden block için kalkan kaldırırsak??
	return true;
}

// Hangisi???
// void UCharacterStateManager::UpdateState(FGameplayTag NewState)
// {
// 	if (!HasState(NewState))
// 	{
// 		// Mevcut aktif state'i kaldır
// 		for (const FGameplayTag& ActiveState : GetActiveStates())
// 		{
// 			RemoveState(ActiveState);
// 		}
//
// 		// Yeni state'i ekle
// 		AddState(NewState);
//
// 		// Event'i tetikle
// 		OnStateChanged.Broadcast(NewState);
//
// 		UE_LOG(LogTemp, Log, TEXT("UpdateState: Yeni durum: %s"), *NewState.ToString());
// 	}
// }