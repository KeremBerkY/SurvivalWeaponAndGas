// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventoryWidget.h"

#include "Components/Image.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/WeaponInventory.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"


void UWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("Weapon Inventory WIDGET Inventory initialize..."));

	
	if (APawn* OwnerPawn = GetOwningPlayerPawn())
	{
		if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(OwnerPawn))
		{
			if (UWeaponInventory* WeaponInventory = PlayerCharacter->GetWeaponInventory())
			{
				BindCallback(WeaponInventory);
				
				if (WeaponInventory->GetDefaultSlotTexture())
				{
					DefaultSlotTexture = PlayerCharacter->GetWeaponInventory()->GetDefaultSlotTexture();
				}
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

void UWeaponInventoryWidget::BindCallback(UWeaponInventory* WeaponInventory)
{
	WeaponInventory->OnResetSlot.AddDynamic(this, &UWeaponInventoryWidget::ResetSlotToDefault);
	WeaponInventory->OnUpdateInventory.AddDynamic(this, &UWeaponInventoryWidget::UpdateInventory);
	
	UE_LOG(LogTemp, Warning, TEXT("Callback functions successfully bound to WeaponInventory!"));
}

void UWeaponInventoryWidget::UpdateInventory(AWeaponBase* Weapon)
{
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is nullptr!"));
		return;
	}

	int32 SlotIndex = GetSlotIndexForCategory(Weapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory);

	if (SlotIndex == INDEX_NONE || SlotIndex >= SlotImages.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SlotIndex for WeaponCategory: %d"), static_cast<int32>(Weapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory));
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

	if (Weapon->GetWeaponDataAsset()->WeaponAttributes.GetWeaponImage())
	{
		SlotImage->SetBrushFromTexture(Weapon->GetWeaponDataAsset()->WeaponAttributes.GetWeaponImage());
		UE_LOG(LogTemp, Log, TEXT("Added new image to SlotIndex: %d for Weapon: %s"), SlotIndex, *Weapon->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon image not found for Weapon: %s"), *Weapon->GetName());
		// SlotImage->SetBrushTintColor(FLinearColor::White); // Varsayılan beyaz renk
	}

	/* ESKİ HALİ */
	// if (Weapon->GetWeaponAttributes().GetWeaponImage())
	// {
	// 	SlotImage->SetBrushFromTexture(Weapon->GetWeaponAttributes().GetWeaponImage());
	// 	UE_LOG(LogTemp, Log, TEXT("Added new image to SlotIndex: %d for Weapon: %s"), SlotIndex, *Weapon->GetName());
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Weapon image not found for Weapon: %s"), *Weapon->GetName());
	// 	// SlotImage->SetBrushTintColor(FLinearColor::White); // Varsayılan beyaz renk
	// }
	
}

void UWeaponInventoryWidget::ResetSlotToDefault(EWeaponCategory WeaponCategory)
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
		UE_LOG(LogTemp, Warning, TEXT("DefaultSlotTexture is nullptr! Please check if it is correctly assigned."));
		return;
	}

	SlotImage->SetBrushFromTexture(DefaultSlotTexture);
	UE_LOG(LogTemp, Warning, TEXT("ResetSlotToDefault CALLED! Set SlotImage to DefaultSlotTexture"));
}


int32 UWeaponInventoryWidget::GetSlotIndexForCategory(EWeaponCategory WeaponCategory) const // TODO: Bunu WeaponInventory'e taşımalı mıyız ??
{
	switch (WeaponCategory) {
	case EWeaponCategory::Ewc_RaycastWeapons:
		return 0; // Slot1Image
		break;
	case EWeaponCategory::Ewc_ProjectileWeapons:
		return 1; // Slot2Image
		break;
	case EWeaponCategory::Ewc_MeleeWeapons:
		return 2; // Slot3Image
		break;
	case EWeaponCategory::Ewc_Max:
		break;
	}
	
	return INDEX_NONE;
}
