// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventoryWidget.h"

#include "Components/Image.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/WeaponInventory.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"


void UWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// if (DefaultSlotImage)
	// {
	// 	DefaultSlotTexture = Cast<UTexture2D>(DefaultSlotImage->GetBrush().GetResourceObject());
	// }

	APawn* OwnerPawn = GetOwningPlayerPawn();
	if (OwnerPawn)
	{
		ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(OwnerPawn);
		if (Character->GetWeaponInventory())
		{
			if (Character->GetWeaponInventory()->GetDefaultSlotTexture())
			{
				DefaultSlotTexture = Character->GetWeaponInventory()->GetDefaultSlotTexture();
			}
		}
	}
	
	SlotImages = { Slot1Image, Slot2Image, Slot3Image };

	for(UImage* SlotImage : SlotImages)
	{
		if (SlotImage && DefaultSlotTexture)
		{
			SlotImage->SetBrushFromTexture(DefaultSlotTexture);
		}
	}
}



void UWeaponInventoryWidget::UpdateInventory(AWeaponBase* Weapon)
{
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is nullptr!"));
		return;
	}

	int32 SlotIndex = GetSlotIndexForCategory(Weapon->WeaponCategory);

	if (SlotIndex == INDEX_NONE || SlotIndex >= SlotImages.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SlotIndex for WeaponCategory: %d"), static_cast<int32>(Weapon->WeaponCategory));
		return;
	}

	UImage* SlotImage = SlotImages[SlotIndex];
	if (!SlotImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("SlotImage is nullptr for SlotIndex: %d"), SlotIndex);
		return;
	}

	if (SlotImage->GetBrush().GetResourceObject() != nullptr)
	{
		SlotImage->SetBrushFromTexture(DefaultSlotTexture);
		UE_LOG(LogTemp, Warning, TEXT("Cleared existing image from SlotIndex: %d"), SlotIndex);
	}

	if (Weapon->WeaponAttributes.GetWeaponImage())
	{
		SlotImage->SetBrushFromTexture(Weapon->WeaponAttributes.GetWeaponImage());
		UE_LOG(LogTemp, Log, TEXT("Added new image to SlotIndex: %d for Weapon: %s"), SlotIndex, *Weapon->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon image not found for Weapon: %s"), *Weapon->GetName());
		// SlotImage->SetBrushTintColor(FLinearColor::White); // Varsayılan beyaz renk
	}
	
}

void UWeaponInventoryWidget::ResetSlotToDefault(EWeaponCategories WeaponCategory)
{
	int32 SlotIndex = GetSlotIndexForCategory(WeaponCategory);

	if (SlotIndex == INDEX_NONE || SlotIndex >= SlotImages.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SlotIndex for WeaponCategory: %d"), static_cast<int32>(WeaponCategory));
		return;
	}

	UImage* SlotImage = SlotImages[SlotIndex];
	if (!SlotImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("SlotImage is nullptr for SlotIndex: %d"), SlotIndex);
		return;
	}
	
	if (!DefaultSlotTexture)
	{
		UE_LOG(LogTemp, Error, TEXT("DefaultSlotTexture is nullptr! Please check if it is correctly assigned."));
		return;
	}

	SlotImage->SetBrushFromTexture(DefaultSlotTexture);
	UE_LOG(LogTemp, Warning, TEXT("ResetSlotToDefault CALLED! Set SlotImage to DefaultSlotTexture"));
}

int32 UWeaponInventoryWidget::GetSlotIndexForCategory(EWeaponCategories WeaponCategory) const
{
	switch (WeaponCategory) {
	case EWeaponCategories::EWC_RaycastWeapons:
		return 0; // Slot1Image
		break;
	case EWeaponCategories::EWC_ProjectileWeapons:
		return 1; // Slot2Image
		break;
	case EWeaponCategories::EWC_MeleeWeapons:
		return 2; // Slot3Image
		break;
	case EWeaponCategories::EWC_MAX:
		break;
	}
	
	return INDEX_NONE;
}