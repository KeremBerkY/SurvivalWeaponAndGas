// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "PistolCrosshair.h"
//
// #include "Components/Image.h"
//
// void UPistolCrosshair::SetCrosshairWidget(ASurvivalCharacter* PlayerCharacter)
// {
// 	Super::SetCrosshairWidget(PlayerCharacter);
// }
//
// void UPistolCrosshair::UpdateCrosshairWidget(float Spread)
// {
// 	Super::UpdateCrosshairWidget(Spread);
//
// 	float FinalSpread = Spread * PistolSpreadMultiplier;
//
// 	if (Top)
// 	{
// 		Top->SetRenderTranslation(FVector2D(0, -FinalSpread)); // Yukarı taşı
// 	}
// 	if (Bottom)
// 	{
// 		Bottom->SetRenderTranslation(FVector2D(0, FinalSpread)); // Aşağı taşı
// 	}
// 	if (Left)
// 	{
// 		Left->SetRenderTranslation(FVector2D(-FinalSpread, 0)); // Sola taşı
// 	}
// 	if (Right)
// 	{
// 		Right->SetRenderTranslation(FVector2D(FinalSpread, 0)); // Sağa taşı
// 	}
// }
//
