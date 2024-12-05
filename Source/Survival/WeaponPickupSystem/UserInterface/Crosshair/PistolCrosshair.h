// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "CrosshairBaseWidget.h"
// #include "PistolCrosshair.generated.h"
//
// class UImage;
//
// UCLASS()
// class SURVIVAL_API UPistolCrosshair : public UCrosshairBaseWidget
// {
// 	GENERATED_BODY()
// 	
// public:
// 	virtual void SetCrosshairWidget(ASurvivalCharacter* PlayerCharacter) override;
// 	virtual void UpdateCrosshairWidget(float Spread) override;
//
// protected:
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair Settings")
// 	float PistolSpreadMultiplier;
// 	
// 	UPROPERTY(meta = (BindWidget))
// 	UImage* Top; 
//
// 	UPROPERTY(meta = (BindWidget))
// 	UImage* Bottom; 
//
// 	UPROPERTY(meta = (BindWidget))
// 	UImage* Left; 
//
// 	UPROPERTY(meta = (BindWidget))
// 	UImage* Right;
// };
