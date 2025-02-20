// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

// #include "Components/CharacterWeaponComponent.h"
#include "CharacterWeaponComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponAttechmentManager/WeaponAttachmentManager.h"



UWeaponInventory::UWeaponInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	UE_LOG(LogTemp, Log, TEXT("Weapon Inventory initialize..."));
	
}

void UWeaponInventory::BeginPlay()
{
	Super::BeginPlay();

	NotifyWeaponInventoryReady();
	InitializeCategoryMap();
}

void UWeaponInventory::NotifyWeaponInventoryReady()
{
	WeaponInventoryReady.Broadcast();
}

void UWeaponInventory::InitializeCategoryMap()
{
	CategoryToSlotMap.Add(EWeaponCategory::Ewc_RaycastWeapons, 1);
	CategoryToSlotMap.Add(EWeaponCategory::Ewc_ProjectileWeapons, 2);
	CategoryToSlotMap.Add(EWeaponCategory::Ewc_MeleeWeapons, 3);
}

void UWeaponInventory::SwapToBackWeapon(AWeaponBase* CurrentWeapon, ASurvivalCharacter* PlayerCharacter, EWeaponCategory DesiredCategory)
{
	// if (!ValidateInputs(PlayerCharacter)) return;
	if (!PlayerCharacter) return;
	
	int32 DesiredSlotIndex = GetSlotIndex(DesiredCategory);
	if (DesiredSlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Desired Weapon Category: %d"), static_cast<int32>(DesiredCategory));
		return;
	}

	AWeaponBase** BackWeaponPtr = WeaponSlots.Find(DesiredSlotIndex);
	AWeaponBase* BackWeapon = BackWeaponPtr ? *BackWeaponPtr : nullptr;

	// TODO: Eğer elimdeki silahın tipinde arkamda silah yoksa bu silahı arkaya ekle doğru slota.

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
	if (CurrentWeapon && BackWeapon && CurrentWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory == DesiredCategory)
	{
		SwapWeapons(CurrentWeapon, BackWeapon, PlayerCharacter, DesiredSlotIndex);
		return;
	}

	// I have a ProjectileWeapon but, I wanted to put a RaycastWeapon by pressing the 1 button.
	if (CurrentWeapon && CurrentWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory != DesiredCategory)
	{
		HandleDifferentCategorySwap(CurrentWeapon, BackWeapon, PlayerCharacter, DesiredSlotIndex, DesiredCategory);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("No valid swap condition met for SwapToBackWeapon."));
}

void UWeaponInventory::EquipBackWeapon(AWeaponBase* BackWeapon, ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex)
{
	if (!BackWeapon || !PlayerCharacter) return;
	
	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(BackWeapon, PlayerCharacter, WeaponSocket ,true);
	
	OnResetSlot.Broadcast(BackWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory);
	UE_LOG(LogTemp, Warning, TEXT("Broadcasting OnResetSlot for category: %d"), static_cast<int32>(BackWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory));

	WeaponSlots.Remove(DesiredSlotIndex);
	UE_LOG(LogTemp, Warning, TEXT("Equipped back weapon: %s"), *BackWeapon->GetName());
}

void UWeaponInventory::MoveCurrentWeaponToBack(AWeaponBase* CurrentWeapon, ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex)
{
	if (!CurrentWeapon || !PlayerCharacter) return;
	
	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(CurrentWeapon, PlayerCharacter, DefaultSocketName, false);
	
	WeaponSlots.Add(DesiredSlotIndex, CurrentWeapon);
	OnUpdateInventory.Broadcast(CurrentWeapon);
	
	UE_LOG(LogTemp, Warning, TEXT("Moved current weapon to back: %s"), *CurrentWeapon->GetName());
}

void UWeaponInventory::SwapWeapons(AWeaponBase* CurrentWeapon, AWeaponBase* BackWeapon, ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex)
{
	if (!CurrentWeapon || !BackWeapon || !PlayerCharacter) return;

	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(CurrentWeapon, PlayerCharacter, DefaultSocketName, false);
	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(BackWeapon, PlayerCharacter, WeaponSocket, true);
	
	WeaponSlots.Remove(DesiredSlotIndex);
	WeaponSlots.Add(DesiredSlotIndex, CurrentWeapon);
	
	OnResetSlot.Broadcast(BackWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory);
	OnUpdateInventory.Broadcast(CurrentWeapon);
	
	UE_LOG(LogTemp, Warning, TEXT("Swapped weapons: %s -> %s"), *CurrentWeapon->GetName(), *BackWeapon->GetName());
}

void UWeaponInventory::HandleDifferentCategorySwap(AWeaponBase* CurrentWeapon, AWeaponBase* BackWeaponForDesiredCategory,ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex, EWeaponCategory DesiredCategory)
{
    if (!CurrentWeapon || !PlayerCharacter) return;
	
    // Find slots by category of current weapon
    // int32 SlotIndex = GetSlotIndex(CurrentWeapon->GetWeaponCategory());
    int32 SlotIndex = GetSlotIndex(CurrentWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory);
    if (SlotIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Current Weapon Category: %d"), static_cast<int32>(CurrentWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory));
        return;
    }

    // Find BackSlot Weapon
    AWeaponBase** BackSlotWeaponForCurrentWeaponCategoryPtr = WeaponSlots.Find(SlotIndex);
    AWeaponBase* BackSlotWeaponForCurrentWeaponCategory = BackSlotWeaponForCurrentWeaponCategoryPtr ? *BackSlotWeaponForCurrentWeaponCategoryPtr : nullptr;

    // If there is a weapon in the BackSlot, drop it
    if (BackSlotWeaponForCurrentWeaponCategory)
    {
		PlayerCharacter->GetCharacterWeaponComponent()->DropWeapon(PlayerCharacter, BackSlotWeaponForCurrentWeaponCategory);
        // OnResetSlot.Broadcast(BackSlotWeaponForCurrentWeaponCategory->GetWeaponCategory());
        OnResetSlot.Broadcast(BackSlotWeaponForCurrentWeaponCategory->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory);
    	
        UE_LOG(LogTemp, Warning, TEXT("DropWeapon() called for: %s"), *BackSlotWeaponForCurrentWeaponCategory->GetName());
    }

    // Add CurrentWeapon to back
	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(CurrentWeapon, PlayerCharacter, DefaultSocketName, false);
    WeaponSlots.Add(SlotIndex, CurrentWeapon);
	
    OnUpdateInventory.Broadcast(CurrentWeapon);
	
    // If you have BackWeaponForDesiredCategory, equip it
    if (BackWeaponForDesiredCategory)
    {
    	PlayerCharacter->GetCharacterWeaponComponent()->EquipWeapon(BackWeaponForDesiredCategory, PlayerCharacter, WeaponSocket, true);
        // OnResetSlot.Broadcast(BackWeaponForDesiredCategory->GetWeaponCategory());
        OnResetSlot.Broadcast(BackWeaponForDesiredCategory->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory);
    	
        WeaponSlots.Remove(DesiredSlotIndex);

        UE_LOG(LogTemp, Warning, TEXT("Swapped weapon: %s to back, and equipped weapon: %s"), *CurrentWeapon->GetName(), *BackWeaponForDesiredCategory->GetName());
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
	// int32* SlotIndexPtr = CategoryToSlotMap.Find(NewWeapon->GetWeaponCategory());
	int32* SlotIndexPtr = CategoryToSlotMap.Find(NewWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory);

	if (!SlotIndexPtr) { UE_LOG(LogTemp, Warning, TEXT("Invalid WeaponCategory: %d"), static_cast<int32>(NewWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory)); return; }

	int32 SlotIndex = *SlotIndexPtr;
	
	// Check if there are any weapons available
	AWeaponBase** ExistingWeaponPtr = WeaponSlots.Find(SlotIndex);
	if (ExistingWeaponPtr)
	{
		AWeaponBase* ExistingWeapon = *ExistingWeaponPtr;
		if (ExistingWeapon)
		{
			// Drop if there is a weapon from the same category
			if (ExistingWeapon && ExistingWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory == NewWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory)
			{
				PlayerCharacter->GetCharacterWeaponComponent()->DropWeapon(PlayerCharacter, ExistingWeapon);
			}
			
			OnResetSlot.Broadcast(NewWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory);
		}
	}
	
	WeaponSlots.Add(SlotIndex, NewWeapon);
	PlayerCharacter->GetCharacterWeaponComponent()->GetWeaponAttachmentManager()->AttachWeaponToSocket(NewWeapon, PlayerCharacter, DefaultSocketName); // Bunu buradaki fonksiyonla çağırmıştık!
	
	OnUpdateInventory.Broadcast(NewWeapon);
	
}

int32 UWeaponInventory::GetSlotIndex(EWeaponCategory Category) const
{
	const int32* SlotIndexPtr = CategoryToSlotMap.Find(Category);
	return SlotIndexPtr ? *SlotIndexPtr : INDEX_NONE;
}

bool UWeaponInventory::HasWeaponInCategory(EWeaponCategory DesiredCategory) const
{
	const int32 DesiredSlotIndex = GetSlotIndex(DesiredCategory);
	
	if (DesiredSlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Desired Weapon Category: %d"), static_cast<int32>(DesiredCategory));
		return false;
	}

	// Slot indeksine karşılık gelen WeaponSlots kontrolü
	AWeaponBase* const* BackWeaponPtr = WeaponSlots.Find(DesiredSlotIndex);

	// Eğer pointer doluysa ve geçerli bir silah işaret ediyorsa, true döndür
	return BackWeaponPtr && *BackWeaponPtr != nullptr;
}

void UWeaponInventory::RemoveFromSlot(AWeaponBase* WeaponToRemove)
{
	if (!WeaponToRemove) 
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponToRemove is nullptr!"));
		return;
	}

	// We find the slot index using the category of WeaponToRemove
	// int32 SlotIndex = GetSlotIndex(WeaponToRemove->GetWeaponCategory());
	int32 SlotIndex = GetSlotIndex(WeaponToRemove->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory);
	if (SlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Weapon Category: %d"), static_cast<int32>(WeaponToRemove->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory));
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


