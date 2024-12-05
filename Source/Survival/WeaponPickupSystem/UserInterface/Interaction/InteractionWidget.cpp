// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Survival/WeaponPickupSystem/PickupSystem/BasePickup.h"
#include "Survival/WeaponPickupSystem/PickupSystem/Interfaces/InteractionInterface.h"

void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.f;
}

void UInteractionWidget::ShowWidget()
{
	if (this && !IsInViewport())
	{
		AddToViewport();
	}
	SetVisibility(ESlateVisibility::Visible);
}

void UInteractionWidget::HideWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData) const
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateWidget called"));

	if (!InteractableData)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractableData is nullptr"));
		return;
	}

	ActionText->SetText(InteractableData->Action);
	NameText->SetText(InteractableData->Name);
	QuantityText->SetText(FText::AsNumber(InteractableData->Quantity));
	
	switch (InteractableData->InteractableType) {
	case EInteractableType::Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if (InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("There is no functionality to show!"))
			// QuantityText->SetText();
			// QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;
	case EInteractableType::Weapon:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EInteractableType::NonPlayerCharacter:
		break;
	case EInteractableType::Device:
		KeyPressText->SetText(FText::FromString("Hold"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Visible);
		break;
	case EInteractableType::Toggle:
		break;
	case EInteractableType::Container:
		break;
	}
	
}

float UInteractionWidget::UpdateInteractionProgress()
{
	return 0.f;
}

