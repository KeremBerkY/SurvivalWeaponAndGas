// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimationsComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"

UWeaponAnimationsComponent::UWeaponAnimationsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponAnimationsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Weapon = Cast<ARaycastWeapons>(GetOwner());
	if (Weapon)
	{
		Weapon->OnInitializedComponents.AddDynamic(this, &UWeaponAnimationsComponent::InitializeAnims);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no Weapon! Weapon = nullptr."));
	}
}

void UWeaponAnimationsComponent::InitializeAnims()
{
	RaycastWeaponData = Weapon->GetRaycastWeaponDataAsset();
	
	Weapon->OnFirePlayMontage.AddDynamic(this, &UWeaponAnimationsComponent::PlayAttackMontage);

}

void UWeaponAnimationsComponent::PlayAttackMontage()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(Character))
	{
		UAnimInstance* AnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && RaycastWeaponData->WeaponAnimMontages.FireMontage)
		{
			AnimInstance->Montage_Play(RaycastWeaponData->WeaponAnimMontages.FireMontage);
			AnimInstance->Montage_JumpToSection(FName("StartFire"));
		}
	}
}

void UWeaponAnimationsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


