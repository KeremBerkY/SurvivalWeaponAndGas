// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalEnemyCharacter.h"

#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Data/EnemyStartupData/DataAsset_EnemyStartUpData.h"
#include "Survival/WeaponPickupSystem/Enemy/EnemyComponents/LockedWidgetComponent.h"
#include "Survival/WeaponPickupSystem/Enemy/EnemyComponents/SelectedWidgetComponent.h"
#include "Survival/WeaponPickupSystem/SharedComponents/Combat/EnemyCombatComponent.h"


ASurvivalEnemyCharacter::ASurvivalEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags.Add(FName("Enemy"));
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	LockedWidgetComponent = CreateDefaultSubobject<ULockedWidgetComponent>(TEXT("LockedWidgetComponent"));
	LockedWidgetComponent->SetupAttachment(RootComponent);

	SelectedWidgetComponent = CreateDefaultSubobject<USelectedWidgetComponent>(TEXT("SelectedWidgetCOmponent"));
	SelectedWidgetComponent->SetupAttachment(RootComponent);

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("HeroCombatComponent"));

}

UPawnCombatComponent* ASurvivalEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

void ASurvivalEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartupData();
}

void ASurvivalEnemyCharacter::InitEnemyStartupData()
{
	if (EnemyStartUpData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		EnemyStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (UDataAsset_EnemyStartUpData* LoadData = EnemyStartUpData.Get())
				{
					LoadData->GiveToAbilitySystemComponent(CharacterAbilitySystemComponent);	
				}
			}
		)
	);
}

