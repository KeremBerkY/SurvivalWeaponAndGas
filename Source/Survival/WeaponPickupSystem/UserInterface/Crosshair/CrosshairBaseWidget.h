// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Blueprint/UserWidget.h"
// #include "CrosshairBaseWidget.generated.h"
//
// class ASurvivalCharacter;
//
// USTRUCT(BlueprintType)
// struct FWeaponCrosshairSettings
// {
// 	GENERATED_USTRUCT_BODY()
//
// 	FWeaponCrosshairSettings() : DefaultSpread(0.0f), MovingSpread(0.0f), SprintingSpread(0.0f) {}
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
// 	float DefaultSpread; // While standing
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
// 	float MovingSpread; // While moving
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
// 	float SprintingSpread; // While running
// 	
// };
//
// UCLASS()
// class SURVIVAL_API UCrosshairBaseWidget : public UUserWidget
// {
// 	GENERATED_BODY()
//
// public:
//
// 	virtual void SetCrosshairWidget(ASurvivalCharacter* PlayerCharacter);
// 	virtual void UpdateCrosshairWidget(float Spread);
//
//
// protected:
// 	virtual void NativeOnInitialized() override;
// 	virtual void NativeConstruct() override;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair Settings")
// 	FWeaponCrosshairSettings CrosshairSettings;
// 	
// };
