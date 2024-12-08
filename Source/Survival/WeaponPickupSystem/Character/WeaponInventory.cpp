// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

#include "CharacterAnimInstance.h"
#include "Components/CharacterWeaponComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponAttechmentManager/WeaponAttachmentManager.h"
#include "Survival/WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival/WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/WeaponInventory/WeaponInventoryWidget.h"


UWeaponInventory::UWeaponInventory()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponInventory::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeCategoryMap();
}

void UWeaponInventory::InitializeCategoryMap()
{
	CategoryToSlotMap.Add(EWeaponCategories::EWC_RaycastWeapons, 1);
	CategoryToSlotMap.Add(EWeaponCategories::EWC_ProjectileWeapons, 2);
	CategoryToSlotMap.Add(EWeaponCategories::EWC_MeleeWeapons, 3);
}

// TODO: HUD'ları değişecek. Delege bağla!

void UWeaponInventory::SwapToBackWeapon(AWeaponBase* CurrentWeapon, ASurvivalCharacter* PlayerCharacter, EWeaponCategories DesiredCategory)
{
	if (!ValidateInputs(PlayerCharacter)) return;
	
	int32 DesiredSlotIndex = GetSlotIndex(DesiredCategory);
	if (DesiredSlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Desired Weapon Category: %d"), static_cast<int32>(DesiredCategory));
		return;
	}

	AWeaponBase** BackWeaponPtr = WeaponSlots.Find(DesiredSlotIndex);
	AWeaponBase* BackWeapon = BackWeaponPtr ? *BackWeaponPtr : nullptr;

	// There is no gun in hand, but there is a gun on your back. So take the one on your back.
	if (!CurrentWeapon && BackWeapon)
	{
		EquipBackWeapon(BackWeapon, PlayerCharacter, DesiredSlotIndex);
		return;
	}

	// There is a gun in hand, but there is no gun of the type I want on my back. Put the gun in hand on my back
	if (CurrentWeapon && !BackWeapon)
	{
		MoveCurrentWeaponToBack(CurrentWeapon, PlayerCharacter, DesiredSlotIndex);
		return;
	}

	// If there is a weapon of the same category on the back, swap them
	if (CurrentWeapon && BackWeapon && CurrentWeapon->WeaponCategory == DesiredCategory)
	{
		SwapWeapons(CurrentWeapon, BackWeapon, PlayerCharacter, DesiredSlotIndex);
		return;
	}

	// I have a ProjectileWeapon but, I wanted to put a RaycastWeapon by pressing the 1 button.
	if (CurrentWeapon && CurrentWeapon->WeaponCategory != DesiredCategory)
	{
		HandleDifferentCategorySwap(CurrentWeapon, BackWeapon, PlayerCharacter, DesiredSlotIndex, DesiredCategory);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("No valid swap condition met for SwapToBackWeapon."));
}

bool UWeaponInventory::ValidateInputs(const ASurvivalCharacter* PlayerCharacter) const
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Character is null"));
		return false;
	}

	if (!PlayerCharacter->GetSurvivalHUD())
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD is null in SwapToBackWeapon"));
		return false;
	}

	return true;
}

void UWeaponInventory::EquipBackWeapon(AWeaponBase* BackWeapon, ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex)
{
	if (!BackWeapon || !PlayerCharacter) return;
	
	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(BackWeapon, PlayerCharacter, WeaponSocket ,true);

	auto HUD = PlayerCharacter->GetSurvivalHUD();
	if (HUD)
	{
		// HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(BackWeapon->GetWeaponCategory());
		OnResetSlot.Broadcast(BackWeapon->GetWeaponCategory());
	}

	WeaponSlots.Remove(DesiredSlotIndex);
	UE_LOG(LogTemp, Warning, TEXT("Equipped back weapon: %s"), *BackWeapon->GetName());
}

void UWeaponInventory::MoveCurrentWeaponToBack(AWeaponBase* CurrentWeapon, ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex)
{
	if (!CurrentWeapon || !PlayerCharacter) return;
	
	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(CurrentWeapon, PlayerCharacter, DefaultSocketName, false);
	
	WeaponSlots.Add(DesiredSlotIndex, CurrentWeapon);

	auto HUD = PlayerCharacter->GetSurvivalHUD();
	if (HUD)
	{
		// HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->UpdateInventory(CurrentWeapon);
		OnUpdateInventory.Broadcast(CurrentWeapon);
	}

	UE_LOG(LogTemp, Warning, TEXT("Moved current weapon to back: %s"), *CurrentWeapon->GetName());
}

void UWeaponInventory::SwapWeapons(AWeaponBase* CurrentWeapon, AWeaponBase* BackWeapon, ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex)
{
	if (!CurrentWeapon || !BackWeapon || !PlayerCharacter) return;

	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(CurrentWeapon, PlayerCharacter, DefaultSocketName, false);
	
	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(BackWeapon, PlayerCharacter, WeaponSocket, true);
	

	WeaponSlots.Remove(DesiredSlotIndex);
	WeaponSlots.Add(DesiredSlotIndex, CurrentWeapon);

	auto HUD = PlayerCharacter->GetSurvivalHUD();
	if (HUD)
	{
		// HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(BackWeapon->GetWeaponCategory());
		OnResetSlot.Broadcast(BackWeapon->GetWeaponCategory());
		
		// HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->UpdateInventory(CurrentWeapon);
		OnUpdateInventory.Broadcast(CurrentWeapon);
	}

	UE_LOG(LogTemp, Warning, TEXT("Swapped weapons: %s -> %s"), *CurrentWeapon->GetName(), *BackWeapon->GetName());
}

void UWeaponInventory::HandleDifferentCategorySwap(AWeaponBase* CurrentWeapon, AWeaponBase* BackWeaponForDesiredCategory,ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex, EWeaponCategories DesiredCategory)
{
    if (!CurrentWeapon || !PlayerCharacter) return;
	
    // Find slots by category of current weapon
    int32 SlotIndex = GetSlotIndex(CurrentWeapon->WeaponCategory);
    if (SlotIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Current Weapon Category: %d"), static_cast<int32>(CurrentWeapon->WeaponCategory));
        return;
    }

    // Find BackSlot Weapon
    AWeaponBase** BackSlotWeaponForCurrentWeaponCategoryPtr = WeaponSlots.Find(SlotIndex);
    AWeaponBase* BackSlotWeaponForCurrentWeaponCategory = BackSlotWeaponForCurrentWeaponCategoryPtr ? *BackSlotWeaponForCurrentWeaponCategoryPtr : nullptr;

    // If there is a weapon in the BackSlot, drop it
    if (BackSlotWeaponForCurrentWeaponCategory)
    {
		PlayerCharacter->GetCharacterWeaponComponent()->DropWeapon(PlayerCharacter, BackSlotWeaponForCurrentWeaponCategory);
    	
        auto HUD = PlayerCharacter->GetSurvivalHUD();
        if (HUD)
        {
            // HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(BackSlotWeaponForCurrentWeaponCategory->GetWeaponCategory());
        	OnResetSlot.Broadcast(BackSlotWeaponForCurrentWeaponCategory->GetWeaponCategory());
        }

        UE_LOG(LogTemp, Warning, TEXT("DropWeapon() called for: %s"), *BackSlotWeaponForCurrentWeaponCategory->GetName());
    }

    // Add CurrentWeapon to back
	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(CurrentWeapon, PlayerCharacter, DefaultSocketName, false);
    WeaponSlots.Add(SlotIndex, CurrentWeapon);

    auto HUD = PlayerCharacter->GetSurvivalHUD();
    if (HUD)
    {
        // HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->UpdateInventory(CurrentWeapon);
    	OnUpdateInventory.Broadcast(CurrentWeapon);
    }

    // If you have BackWeaponForDesiredCategory, equip it
    if (BackWeaponForDesiredCategory)
    {
    	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(BackWeaponForDesiredCategory, PlayerCharacter, WeaponSocket, true);

        if (HUD)
        {
            // HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(BackWeaponForDesiredCategory->GetWeaponCategory());
        	OnResetSlot.Broadcast(BackWeaponForDesiredCategory->GetWeaponCategory());
        }

        WeaponSlots.Remove(DesiredSlotIndex);

        UE_LOG(LogTemp, Warning, TEXT("Swapped weapon: %s to back, and equipped weapon: %s"), 
            *CurrentWeapon->GetName(), *BackWeaponForDesiredCategory->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No weapon found in DesiredCategory slot to equip."));
    }
}

void UWeaponInventory::AddWeaponToSlot(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter)
{
	if (!NewWeapon) { UE_LOG(LogTemp, Warning, TEXT("NewWeapon is nullptr!")); return; }

	if (CategoryToSlotMap.Num() == 0)
	{
		InitializeCategoryMap();
	}
	
	// Finding the slot number corresponding to the category with SlotIndexPtr
	int32* SlotIndexPtr = CategoryToSlotMap.Find(NewWeapon->WeaponCategory);

	if (!SlotIndexPtr) { UE_LOG(LogTemp, Warning, TEXT("Invalid WeaponCategory: %d"), static_cast<int32>(NewWeapon->WeaponCategory)); return; }

	int32 SlotIndex = *SlotIndexPtr;
	
	// Check if there are any weapons available
	AWeaponBase** ExistingWeaponPtr = WeaponSlots.Find(SlotIndex);
	if (ExistingWeaponPtr)
	{
		AWeaponBase* ExistingWeapon = *ExistingWeaponPtr;

		if (ExistingWeapon)
		{
			// Drop if there is a weapon from the same category
			if (ExistingWeapon && ExistingWeapon->WeaponCategory == NewWeapon->WeaponCategory)
			{
				PlayerCharacter->GetCharacterWeaponComponent()->DropWeapon(PlayerCharacter, ExistingWeapon);
			}

			ASurvivalSystemHUD* HUD = PlayerCharacter->GetSurvivalHUD();
			if (HUD)
			{
				// HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(NewWeapon->WeaponCategory);
				OnResetSlot.Broadcast(NewWeapon->WeaponCategory);
			}
		}
	}
	
	WeaponSlots.Add(SlotIndex, NewWeapon);
	PlayerCharacter->GetCharacterWeaponComponent()->GetWeaponAttachmentManager()->AttachWeaponToSocket(NewWeapon, PlayerCharacter, DefaultSocketName); // Bunu buradaki fonksiyonla çağırmıştık!
	
	ASurvivalSystemHUD* HUD = PlayerCharacter->GetSurvivalHUD();
	if (HUD)
	{
		// HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->UpdateInventory(NewWeapon);
		OnUpdateInventory.Broadcast(NewWeapon);
	}
}

int32 UWeaponInventory::GetSlotIndex(EWeaponCategories Category) const
{
	const int32* SlotIndexPtr = CategoryToSlotMap.Find(Category);
	return SlotIndexPtr ? *SlotIndexPtr : INDEX_NONE;
}

void UWeaponInventory::RemoveFromSlot(AWeaponBase* WeaponToRemove)
{
	if (!WeaponToRemove) 
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponToRemove is nullptr!"));
		return;
	}

	// We find the slot index using the category of WeaponToRemove
	int32 SlotIndex = GetSlotIndex(WeaponToRemove->WeaponCategory);
	if (SlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Weapon Category: %d"), static_cast<int32>(WeaponToRemove->WeaponCategory));
		return;
	}
	
	// Check if there is a weapon in the given index in WeaponSlots
	if (WeaponSlots.Contains(SlotIndex) && WeaponSlots[SlotIndex] == WeaponToRemove)
	{
		WeaponSlots.Remove(SlotIndex);
		UE_LOG(LogTemp, Warning, TEXT("Weapon removed from slot: %s"), *WeaponToRemove->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon not found in slot for removal."));
	}
}


