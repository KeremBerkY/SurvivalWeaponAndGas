// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairBaseWidget.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"


void UCrosshairBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCrosshairBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwnerPawn = GetOwningPlayerPawn())
	{
		if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(OwnerPawn))
		{
			if (UCharacterCameraComponent* CameraComponent = PlayerCharacter->GetCharacterCameraComponent())
			{
				CameraComponent->OnAiming.AddDynamic(this, &UCrosshairBaseWidget::SetCrosshairWidget);
			}
		}
	}
}

void UCrosshairBaseWidget::SetCrosshairWidget(ASurvivalCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	if (PlayerCharacter->GetCharacterCameraComponent()->IsAiming())
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCrosshairBaseWidget::UpdateCrosshairWidget(float Spread)
{
	
}