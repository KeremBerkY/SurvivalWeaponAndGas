// Fill out your copyright notice in the Description page of Project Settings.


#include "LockonComponent.h"

#include "CharacterCameraComponent.h"
#include "CharacterWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalEnemyCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "Survival/WeaponPickupSystem/Enemy/EnemyBase.h"
#include "Survival/WeaponPickupSystem/Enemy/EnemyComponents/LockedWidgetComponent.h"
#include "Survival/WeaponPickupSystem/Enemy/EnemyComponents/SelectedWidgetComponent.h"
#include "Survival/WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponRangedWidgets/RaycastWeaponWidget/RaycastCurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/Lockon/FocusCrosshair/FocusCrosshair.h"


ULockonComponent::ULockonComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsLocked = false;
	CurrentTargetActor = nullptr;
	bIsNotRaycast = false;
}

void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacterPtr = Cast<ASurvivalCharacter>(GetOwner());

	ControllerPtr = GetWorld()->GetFirstPlayerController();
	MovementComponent = PlayerCharacterPtr->GetCharacterMovement();

	FocusCrosshairInitialize();
}

void ULockonComponent::FocusCrosshairInitialize()
{
	if (FocusCrosshairClass)
	{
		FocusCrosshair = CreateWidget<UFocusCrosshair>(GetWorld(), FocusCrosshairClass);
		if (FocusCrosshair)
		{
			FocusCrosshair->AddToViewport();
		}
	}
}

void ULockonComponent::RemoveFocusCrosshair() const
{
	if (FocusCrosshair)
	{
		FocusCrosshair->HideFocusCrosshair();
	}
}

void ULockonComponent::AddFocusCrosshair() const
{
	if (FocusCrosshair && !bIsLocked)
	{
		FocusCrosshair->ShowFocusCrosshair();
	}
}

void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckCurrentWeaponAndCategory();
	
	if (!bIsNotRaycast)
	{
		AddFocusCrosshair();
	}
	else
	{
		RemoveFocusCrosshair();
		return;
	}
	
	CheckAndPerformTargetSelection(DeltaTime);
	
	RotateTowardsTarget(DeltaTime);
	
}


void ULockonComponent::StartLockon() // TODO: Bu rakip çok yakına gelirse çalıştıracağımız bir şey olabilir
{
	if (!PlayerCharacterPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacterPtr is invalid!"));
		return;
	}
	
	FHitResult OutResult;
	
	FVector StartLocation = PlayerCharacterPtr->GetCharacterCameraComponent()->GetComponentLocation();
	FVector ForwardVector = PlayerCharacterPtr->GetCharacterCameraComponent()->GetForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * 10000.0f);
	
	FCollisionQueryParams IgnoreParams{
		FName{ TEXT("Ignore Collision Params") },
		false,
		PlayerCharacterPtr.Get()
	};

	// ECC_GameTraceChannel1
	bool bHasFoundTarget = GetWorld()->LineTraceSingleByChannel(
		OutResult,
		StartLocation,
		EndLocation,
		ECC_Visibility, // TODO: Visibility diye ayarla!
		IgnoreParams
	);

	if (bHasFoundTarget)
	{
		AActor* HitActor = OutResult.GetActor();
		if (HitActor->ActorHasTag(FName("Enemy"))) // Nereden bu tag veriliyor?
		{
			UE_LOG(LogTemp, Warning, TEXT("Target Found: %s"), *HitActor->GetName());
			
			// CurrentTargetActor = Cast<AEnemyBase>(HitActor); // For TEST Enemy
			CurrentTargetActor = Cast<ASurvivalEnemyCharacter>(HitActor);
			bIsLocked = true;
			
			if (CurrentTargetActor)
			{
				// FocusCrosshair->HideFocusCrosshair();
				RemoveFocusCrosshair();
				EndSelect();
				UpdateTargetState(ETargetWidgetState::GetOutFromTarget);
				CurrentTargetActor->GetLockedWidgetComponent()->ShowLockWidget();
			}
			
			ControllerPtr.Get()->SetIgnoreLookInput(true);
			MovementComponent.Get()->bOrientRotationToMovement = false;
			MovementComponent.Get()->bUseControllerDesiredRotation = true;

			PlayerCharacterPtr->GetCameraBoom()->TargetOffset = FVector(0.f, 0.f, 100.f); // TODO: Smooth hale getir!
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No target found!"));
	}
	
	DrawDebugVisuals(StartLocation, EndLocation, OutResult);

}

void ULockonComponent::EndLockon()
{
	if (CurrentTargetActor)
	{
		FocusCrosshair->ShowFocusCrosshair();
		CurrentTargetActor->GetLockedWidgetComponent()->HideLockWidget();
	}
	
	CurrentTargetActor = nullptr;
	SetLocked(false);
	
	MovementComponent.Get()->bOrientRotationToMovement = true;
	MovementComponent.Get()->bUseControllerDesiredRotation = false;
	PlayerCharacterPtr->GetCameraBoom()->TargetOffset = FVector::ZeroVector;

	ControllerPtr.Get()->ResetIgnoreLookInput();
	
}

void ULockonComponent::PerformSelect()
{
	if (auto Camera = PlayerCharacterPtr->GetCharacterCameraComponent())
	{
		FVector Start = Camera->GetComponentLocation();
		FVector ForwardDirection = Camera->GetForwardVector();
		FVector End = Start + (ForwardDirection * 10000.f);
		
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(PlayerCharacterPtr.Get());

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			DrawDebugVisuals(Start, End, HitResult);
			
			// if(AEnemyBase* HitEnemy = Cast<AEnemyBase>(HitResult.GetActor()))
			if(ASurvivalEnemyCharacter* HitEnemy = Cast<ASurvivalEnemyCharacter>(HitResult.GetActor()))
			{
				if (SelectedActor != HitEnemy)
				{
					EndSelect();

					SelectedActor = HitEnemy;
					if (!PlayerCharacterPtr->GetCharacterCameraComponent()->IsAiming())
					{
						SelectedActor->GetSelectedWidgetComponent()->ShowSelectedWidget();
					}
					UpdateTargetState(ETargetWidgetState::TargetFound);
				}
			}
			else if (SelectedActor)
			{
				EndSelect();
				UpdateTargetState(ETargetWidgetState::NoTargetFound);
				SelectedActor = nullptr;
			}
		}
		else if (SelectedActor)
		{
			DrawDebugVisuals(Start, End, HitResult);
			
			EndSelect();
			SelectedActor = nullptr;
		}
	}
}

void ULockonComponent::EndSelect() const
{
	if (SelectedActor)
	{
		SelectedActor->GetSelectedWidgetComponent()->HideSelectedWidget();
	}
}

void ULockonComponent::CheckCurrentWeaponAndCategory()
{
	if (!PlayerCharacterPtr.IsValid()) {return;}

	const auto CurrentWeapon = PlayerCharacterPtr.Get()->GetCharacterWeaponComponent()->GetCurrentWeapon();
	
	if (!CurrentWeapon || CurrentWeapon->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory != EWeaponCategory::Ewc_RaycastWeapons)
	{
		bIsNotRaycast = true;
	}
	else
	{
		bIsNotRaycast = false;
	}
	
}

void ULockonComponent::CheckAndPerformTargetSelection(float DeltaTime)
{
	if (!IsValid(CurrentTargetActor)) // TODO:  && !PlayerCharacterPtr->GetCharacterCameraComponent()->IsAiming() Bunu düşün!
	{
		static float TimeSinceLastCheck = 0.0f;
		constexpr float Interval = 0.25f;

		TimeSinceLastCheck += DeltaTime;

		if (TimeSinceLastCheck >= Interval)
		{
			TimeSinceLastCheck = 0.0f;
			PerformSelect();
		}
	}
}

void ULockonComponent::UpdateTargetState(const ETargetWidgetState NewState) const
{
	switch (NewState)
	{
	case ETargetWidgetState::NoTargetFound:
		PlayerCharacterPtr.Get()->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()->GetCurrentWeaponWidget()->GetRaycastCurrentWeaponWidget()->SetWidgetState(ETargetWidgetState::NoTargetFound);
		break;
	case ETargetWidgetState::TargetFound:
		PlayerCharacterPtr.Get()->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()->GetCurrentWeaponWidget()->GetRaycastCurrentWeaponWidget()->SetWidgetState(ETargetWidgetState::TargetFound);
		break;
	case ETargetWidgetState::GetOutFromTarget:
		PlayerCharacterPtr.Get()->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()->GetCurrentWeaponWidget()->GetRaycastCurrentWeaponWidget()->SetWidgetState(ETargetWidgetState::GetOutFromTarget);
		break;
	}
}

void ULockonComponent::RotateTowardsTarget(float DeltaTime) const
{
	if (!IsValid(CurrentTargetActor) || !PlayerCharacterPtr.IsValid()) { return; }

	const FVector CurrentLocation = PlayerCharacterPtr.Get()->GetActorLocation();
	FVector TargetLocation = CurrentTargetActor->GetActorLocation();

	TargetLocation.Z -= 125;
	
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	if (ControllerPtr.IsValid())
	{
		const FRotator CurrentRotation = ControllerPtr.Get()->GetControlRotation();

		constexpr float InterpSpeed = 5.0f;
		const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);

		ControllerPtr->SetControlRotation(NewRotation);
	}
}

void ULockonComponent::DrawDebugVisuals(const FVector& Start, const FVector& End, const FHitResult& HitResult) const
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 1.0f, 0, 2.0f);
	
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Green, false, 1.0f);

		if (const AActor* HitActor = HitResult.GetActor())
		{
			// UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Impact Point: %s"), *HitActor->GetName(), *HitResult.ImpactPoint.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No actor hit. Impact Point: %s"), *HitResult.ImpactPoint.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Raycast did not hit any surface."));
	}
}


