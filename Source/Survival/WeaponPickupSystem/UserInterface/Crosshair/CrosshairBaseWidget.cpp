// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "CrosshairBaseWidget.h"
//
// #include "Survival/SurvivalCharacter.h"
// #include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
// #include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"
// #include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
//
//
// void UCrosshairBaseWidget::NativeOnInitialized()
// {
// 	Super::NativeOnInitialized();
// }
//
// void UCrosshairBaseWidget::NativeConstruct()
// {
// 	Super::NativeConstruct();
// }
//
// void UCrosshairBaseWidget::SetCrosshairWidget(ASurvivalCharacter* PlayerCharacter)
// {
// 	if (!PlayerCharacter) return;
// 	
// 	ASurvivalSystemHUD* HUD = PlayerCharacter->GetSurvivalHUD();
// 	if (!HUD) return;
// 	
// 	auto CurrentCrosshairWidget = HUD->GetCurrentCrosshairWidget();
// 	
// 	// AWeaponBase* CurrentWeapon = PlayerCharacter->GetCurrentWeapon();
// 	AWeaponBase* CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon();
// 	if (!CurrentWeapon || !CurrentWeapon->WeaponAttributes.CrosshairWidget)
// 	{
// 		// if there is no weapon put the gun away
// 		if (CurrentCrosshairWidget)
// 		{
// 			CurrentCrosshairWidget->RemoveFromParent();
// 			HUD->SetCurrentCrosshairWidget(nullptr);
// 		}
// 		return;
// 	}
//
// 	
// 	if (CurrentCrosshairWidget && CurrentCrosshairWidget->GetClass() == CurrentWeapon->WeaponAttributes.CrosshairWidget) 
// 	{
// 		return; // Do not perform any action if the correct crosshair widget is already set
// 	}
// 	
// 	if (CurrentCrosshairWidget)
// 	{
// 		CurrentCrosshairWidget->RemoveFromParent();
// 		HUD->SetCurrentCrosshairWidget(nullptr);
// 	}
// 	
// 	CurrentCrosshairWidget = CreateWidget<UCrosshairBaseWidget>(GetWorld(), CurrentWeapon->WeaponAttributes.CrosshairWidget);
// 	if (CurrentCrosshairWidget)
// 	{
// 		CurrentCrosshairWidget->AddToViewport();
// 		HUD->SetCurrentCrosshairWidget(CurrentCrosshairWidget);
// 	}
// }
//
// void UCrosshairBaseWidget::UpdateCrosshairWidget(float Spread)
// {
// 	
// }