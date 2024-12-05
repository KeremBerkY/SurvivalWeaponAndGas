// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

#include "CharacterAnimInstance.h"
#include "Components/CharacterWeaponComponent.h"
#include "Survival/SurvivalCharacter.h"
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

void UWeaponInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UWeaponInventory::InitializeCategoryMap()
{
	UE_LOG(LogTemp, Error, TEXT("CategoryToSlotMap is empty. Initializing map..."));
	CategoryToSlotMap.Add(EWeaponCategories::EWC_RaycastWeapons, 1);
	CategoryToSlotMap.Add(EWeaponCategories::EWC_ProjectileWeapons, 2);
	CategoryToSlotMap.Add(EWeaponCategories::EWC_MeleeWeapons, 3);
}

void UWeaponInventory::AddWeaponToSlot(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter)
{
	if (!NewWeapon) { UE_LOG(LogTemp, Warning, TEXT("NewWeapon is nullptr!")); return; }

	if (CategoryToSlotMap.Num() == 0)
	{
		InitializeCategoryMap();
	}
	
	// SlotIndexPtr ile kategoriye karşılık gelen slot numarasını bulma
	int32* SlotIndexPtr = CategoryToSlotMap.Find(NewWeapon->WeaponCategory);

	if (!SlotIndexPtr) { UE_LOG(LogTemp, Warning, TEXT("Invalid WeaponCategory: %d"), static_cast<int32>(NewWeapon->WeaponCategory)); return; }

	int32 SlotIndex = *SlotIndexPtr;
	
	// Mevcut silah var mı kontrolü
	AWeaponBase** ExistingWeaponPtr = WeaponSlots.Find(SlotIndex);
	if (ExistingWeaponPtr)
	{
		AWeaponBase* ExistingWeapon = *ExistingWeaponPtr;

		if (ExistingWeapon)
		{
			// Aynı kategoriden silah varsa drop et
			if (ExistingWeapon && ExistingWeapon->WeaponCategory == NewWeapon->WeaponCategory)
			{
				ExistingWeapon->DropWeapon(PlayerCharacter);
			}

			ASurvivalSystemHUD* HUD = PlayerCharacter->GetSurvivalHUD();
			if (HUD)
			{
				// HUD->ResetSlotToDefault(NewWeapon->WeaponCategory);
				HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(NewWeapon->WeaponCategory);
			}
		}
	}
	
	WeaponSlots.Add(SlotIndex, NewWeapon);
	AttachWeaponToSocket(NewWeapon, PlayerCharacter, NameOfTheWeaponAccordingToType);
	
	ASurvivalSystemHUD* HUD = PlayerCharacter->GetSurvivalHUD();
	if (HUD)
	{
		// HUD->UpdateWeaponInventory(NewWeapon);
		HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->UpdateInventory(NewWeapon);
	}
}



void UWeaponInventory::AttachWeaponToSocket(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter, FName OverrideSocketName)
{
	if (!NewWeapon || !PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachWeaponToSocket called! but there is no Weapon!!"))
		return;
	}

	FName SocketName = (OverrideSocketName != NameOfTheWeaponAccordingToType) ? OverrideSocketName : NewWeapon->GetWeaponSocketName(PlayerCharacter);
	UE_LOG(LogTemp, Warning, TEXT("AttachWeaponToSocket called! SocketName: %s"), *SocketName.ToString())
	
	// FName SocketName = NewWeapon->GetWeaponSocketName(PlayerCharacter);
	switch (NewWeapon->WeaponCategory)
	{
	case EWeaponCategories::EWC_RaycastWeapons:
		NewWeapon->AttechMeshToSocket(PlayerCharacter->GetMesh(), SocketName);
		break;
	case EWeaponCategories::EWC_ProjectileWeapons:
		NewWeapon->AttechMeshToSocket(PlayerCharacter->GetMesh(), SocketName);
		break;
	case EWeaponCategories::EWC_MeleeWeapons:
		UE_LOG(LogTemp, Warning, TEXT("Melee Weapon added to socket"))
		break;
	case EWeaponCategories::EWC_MAX:
		UE_LOG(LogTemp, Warning, TEXT("Cannot add to socket! Empty NewWeapon Category!!"))
		break;
	default: ;
	}
}

// TODO: Test et çalışıyor mu! Daha 1,2,3 tuşlarına silah kategorilerini koyacaksın ve fonksiyon yazarken check et eğer CurrentWeapon = RaycasWeapon ise ve 1 tuşuna karşılık gelen RaycastWeapon'a tıkladıysan yer değiştirsinler. 
void UWeaponInventory::SwapToBackWeapon(AWeaponBase* CurrentWeapon, ASurvivalCharacter* PlayerCharacter, EWeaponCategories DesiredCategory)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null in SwapToBackWeapon"));
		return;
	}

	ASurvivalSystemHUD* HUD = PlayerCharacter->GetSurvivalHUD();
	if (!HUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD is null in UpdateHUDForWeaponSwap!"));
		return;
	}
	
	InitializeCategoryMap();

	int32 DesiredSlotIndex = GetSlotIndex(DesiredCategory);
	if (DesiredSlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Desired Weapon Category: %d"), static_cast<int32>(DesiredCategory));
		return;
	}
	
	AWeaponBase** BackWeaponForDesiredCategoryPtr = WeaponSlots.Find(DesiredSlotIndex);
	AWeaponBase* BackWeaponForDesiredCategory = BackWeaponForDesiredCategoryPtr ? *BackWeaponForDesiredCategoryPtr : nullptr;

	
	if (!BackWeaponForDesiredCategory && (!CurrentWeapon || CurrentWeapon->WeaponCategory != DesiredCategory))
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon available in desired category, and current weapon does not match the category."));
		return;
	}

	// Elde silah yok, ama sırtta silah var. O zaman sırttakini ele al.
	if (!CurrentWeapon && BackWeaponForDesiredCategory)
	{
		EquipWeapon(BackWeaponForDesiredCategory, PlayerCharacter, WeaponSocket, true);
		
		// HUD->ResetSlotToDefault(BackWeaponForDesiredCategory->GetWeaponCategory()); // Slotu sıfırlamak için HUD üzerinden ResetSlotToDefault çağrılır
		HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(BackWeaponForDesiredCategory->GetWeaponCategory());
		WeaponSlots.Remove(DesiredSlotIndex);
		
		UE_LOG(LogTemp, Warning, TEXT("Equipped back weapon: %s"), *BackWeaponForDesiredCategory->GetName());
		return;
	}

	// Elde silah var, ama sırtta istediğim tipte silah yok. Eldeki silahı sırta koy
	if (CurrentWeapon && !BackWeaponForDesiredCategory)
	{
		EquipWeapon(CurrentWeapon, PlayerCharacter, NameOfTheWeaponAccordingToType, false);
		
		WeaponSlots.Add(DesiredSlotIndex, CurrentWeapon);
		// HUD->UpdateWeaponInventory(CurrentWeapon); // Eldeki silahı sırta ekle
		HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->UpdateInventory(CurrentWeapon);
		
		UE_LOG(LogTemp, Warning, TEXT("Moved current weapon to back and added to WeaponSlots: %s"), *CurrentWeapon->GetName());
	}

	// Eğer sırtta aynı kategoriden bir silah varsa, onları yer değiştir
	if (CurrentWeapon && BackWeaponForDesiredCategory && CurrentWeapon->WeaponCategory == DesiredCategory)
	{
		EquipWeapon(CurrentWeapon, PlayerCharacter, NameOfTheWeaponAccordingToType, false); // Elimizdeki silahı sırt soketine koy
		EquipWeapon(BackWeaponForDesiredCategory, PlayerCharacter, WeaponSocket, true); // Sırttaki silahı ele al

		WeaponSlots.Remove(DesiredSlotIndex);
		WeaponSlots.Add(DesiredSlotIndex, CurrentWeapon);
		
		// HUD->ResetSlotToDefault(BackWeaponForDesiredCategory->GetWeaponCategory()); // Sırttaki silahın slotunu sıfırla
		HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(BackWeaponForDesiredCategory->GetWeaponCategory());
		// HUD->UpdateWeaponInventory(CurrentWeapon); // Eldeki silahı sırta ekle
		HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->UpdateInventory(CurrentWeapon);
		// TODO: HUD-> // Sırttaki silahı ele al
		
		UE_LOG(LogTemp, Warning, TEXT("Swapped weapon: %s to back, and equipped weapon: %s"), *CurrentWeapon->GetName(), *BackWeaponForDesiredCategory->GetName());
	}

	// Elimde projectile var ama ben 1 tuşuna basıp raycast koymak istedim.
	if (CurrentWeapon && CurrentWeapon->WeaponCategory != DesiredCategory)
	{
		InitializeCategoryMap();
		UE_LOG(LogTemp, Warning, TEXT("Elimde projectile var ama ben 1 tuşuna basıp raycast koymak istedim."));

		int32 SlotIndex = GetSlotIndex(CurrentWeapon->WeaponCategory);
		if (SlotIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Desired Weapon Category: %d"), static_cast<int32>(DesiredCategory));
			return;
		}
		AWeaponBase** BackSlotWeaponForCurrentWeaponCategoryPtr = WeaponSlots.Find(SlotIndex);
		AWeaponBase* BackSlotWeaponForCurrentWeaponCategory = BackSlotWeaponForCurrentWeaponCategoryPtr ? *BackSlotWeaponForCurrentWeaponCategoryPtr : nullptr;

		// Eğer elimizdeki silahın tipinde bir silah varsa backslotumuzda yere at ve currentweapon'u backslota yerleştir. Sonra elimiz unarmed kalsın ??
		if (BackSlotWeaponForCurrentWeaponCategory) 
		{
			BackSlotWeaponForCurrentWeaponCategory->DropWeapon(PlayerCharacter);
			// HUD->ResetSlotToDefault(BackSlotWeaponForCurrentWeaponCategory->GetWeaponCategory());
			HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(BackSlotWeaponForCurrentWeaponCategory->GetWeaponCategory());
			
			UE_LOG(LogTemp, Warning, TEXT("DropWeapon() called!"));
		}
		
		EquipWeapon(CurrentWeapon, PlayerCharacter, NameOfTheWeaponAccordingToType, false);
		
		WeaponSlots.Add(SlotIndex, CurrentWeapon);
		// HUD->UpdateWeaponInventory(CurrentWeapon);
		HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->UpdateInventory(CurrentWeapon);
		
		if (BackWeaponForDesiredCategory)
		{
			EquipWeapon(BackWeaponForDesiredCategory, PlayerCharacter, WeaponSocket, true);

			// HUD->ResetSlotToDefault(BackWeaponForDesiredCategory->GetWeaponCategory());
			HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetWeaponInventoryWidget()->ResetSlotToDefault(BackWeaponForDesiredCategory->GetWeaponCategory());
			
			WeaponSlots.Remove(DesiredSlotIndex);
			UE_LOG(LogTemp, Warning, TEXT("Swapped weapon: %s to back, and equipped weapon: %s"), *CurrentWeapon->GetName(), *BackWeaponForDesiredCategory->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No weapon found in DesiredCategory slot to equip."));
		}	
	}
}

int32 UWeaponInventory::GetSlotIndex(EWeaponCategories Category) const
{
	const int32* SlotIndexPtr = CategoryToSlotMap.Find(Category);
	return SlotIndexPtr ? *SlotIndexPtr : INDEX_NONE;
}

void UWeaponInventory::EquipWeapon(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter, FName SocketName, bool bSetAsCurrent = true) // TODO: Bunu CharacterWeaponComponent'a taşı
{
	if (Weapon)
	{
		AttachWeaponToSocket(Weapon, PlayerCharacter, SocketName);
		
		// bSetAsCurrent true ise, bu silahı CurrentWeapon olarak ayarla; değilse nullptr yap
		// PlayerCharacter->SetCurrentWeapon(bSetAsCurrent ? Weapon : nullptr);
		PlayerCharacter->GetCharacterWeaponComponent()->SetCurrentWeapon(bSetAsCurrent ? Weapon : nullptr);
		
		UCharacterAnimInstance* AnimInstance = PlayerCharacter->GetCharacterAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->UpdateWeaponType(Weapon);
		}

		UE_LOG(LogTemp, Warning, TEXT("Equipped weapon: %s to socket: %s"), *Weapon->GetName(), *SocketName.ToString());

		// TODO: Buraya elimizdeki silahın CrosshairWidget'ını ekleyeceğiz örn; RifleCrosshair
		// ASurvivalSystemHUD* HUD = PlayerCharacter->GetSurvivalHUD();
		// if (HUD && Weapon->GetCrosshairWidgetClass())
		// {
		// 	// TODO: Burada elimizdeki silahın widgetsınıfını alıp viewporta ekleyeceğiz ve diğer silahın viewvportunu kapatacağız.
		// 	// HUD->SetCrosshairClass(Weapon);
		// }
	}
}

void UWeaponInventory::RemoveFromSlot(AWeaponBase* WeaponToRemove)
{
	if (!WeaponToRemove) 
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponToRemove is nullptr!"));
		return;
	}

	// WeaponToRemove'un kategorisini kullanarak slot indeksini buluyoruz
	int32 SlotIndex = GetSlotIndex(WeaponToRemove->WeaponCategory);
	if (SlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Weapon Category: %d"), static_cast<int32>(WeaponToRemove->WeaponCategory));
		return;
	}
	
	// WeaponSlots içinde verilen indekste bir silah olup olmadığını kontrol et
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

void UWeaponInventory::UpdateHUDForWeaponSwap(AWeaponBase* CurrentWeapon, AWeaponBase* BackWeapon, ASurvivalCharacter* PlayerCharacter, EWeaponCategories DesiredCategory)
{
	
}


