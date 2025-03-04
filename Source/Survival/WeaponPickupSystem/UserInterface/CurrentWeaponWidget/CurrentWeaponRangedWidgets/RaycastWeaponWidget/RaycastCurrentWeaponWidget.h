// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaycastCurrentWeaponWidget.generated.h"

class UWeaponFireModesWidget;
class ARaycastWeapons;
class UCurrentWeaponWidget;
class URaycastWeaponUIHandler;
class ASurvivalCharacter;
class UAmmoWidget;
class UNoTargetFoundWidget;
class UTargetFoundWidget;
class UGetOutFromTargetWidget;
class UWidgetSwitcher;
class UTextBlock;
class UImage;
class URadialSliderWidget;


UENUM(BlueprintType)
enum class ETargetWidgetState : uint8
{
	NoTargetFound,
	TargetFound,
	GetOutFromTarget
};


UCLASS()
class SURVIVAL_API URaycastCurrentWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE UGetOutFromTargetWidget* GetGetOutFromTargetWidget() const { return GetOutFromTargetWidget; }
	FORCEINLINE UWeaponFireModesWidget* GetWeaponFireModesWidget() const { return WeaponFireModesWidget; }
	
	// void NativeOnInitialized();
	void SetParent(UCurrentWeaponWidget* WeaponWidget);
	void BindUICallbacks(ARaycastWeapons* RaycastWeapon);

	UFUNCTION()
	void ShowCurrentWeaponRaycastWidget();
	void HideCurrentWeaponRaycastWidget();

	void SetCurrentWeaponImage(UTexture2D* WeaponImage);

	UFUNCTION()
	void InitializeAmmo();

	void SetWidgetState(ETargetWidgetState NewState);

private:

	// Using for FocusCrosshair Targeting states
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UNoTargetFoundWidget* NoTargetFoundWidget;
	
	UPROPERTY(meta = (BindWidget))
	UTargetFoundWidget* TargetFoundWidget;
	
	UPROPERTY(meta = (BindWidget))
	UGetOutFromTargetWidget* GetOutFromTargetWidget;
	
	UPROPERTY(meta = (BindWidget))
	UImage* CurrentWeaponImage;

	UPROPERTY(meta = (BindWidget))
	UAmmoWidget* AmmoWidget;

	UPROPERTY(meta = (BindWidget))
	UWeaponFireModesWidget* WeaponFireModesWidget;

	// TWeakObjectPtr<ASurvivalCharacter> PlayerCharacterPtr;
	TWeakObjectPtr<UCurrentWeaponWidget> CurrentWeaponWidgetPtr;
	TWeakObjectPtr<ARaycastWeapons> RaycastWeaponPtr;
	

};
