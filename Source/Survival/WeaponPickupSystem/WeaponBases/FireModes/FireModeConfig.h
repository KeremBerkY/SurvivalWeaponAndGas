#pragma once

// #pragma once

#include "CoreMinimal.h"
#include "FireModeConfig.generated.h"


UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SingleShot UMETA(DisplayName = "Single Shot"),
	BurstFire UMETA(DisplayName = "Burst Fire"),
	Automatic UMETA(DisplayName = "Automatic")
};

USTRUCT(BlueprintType)
struct FFireSettings
{
	GENERATED_USTRUCT_BODY()

	FFireSettings() : bIsFiring(false), FireRate(0), bIsAutomatic(false), bBurstFiring(false), bIsCooldownActive(false),
	                  bPendingFire(false), bCanFire(true), BurstCount(3), BurstShotsRemaining(0), CurrentFireModeIndex(0),
	                  SingleShotName("Single Shot"), BurstFireName("Burst Fire"), AutomaticName("Automatic")
	{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Fire Settings")
	bool bIsFiring;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Fire Settings")
	float FireRate; // Silah ateşleme hızı, saniyede kaç atış yapılabileceğini belirler

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Fire Settings")
	bool bIsAutomatic; // Otomatik ateşleme desteklenip desteklenmediği

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Fire Settings")
	bool bBurstFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Fire Settings")
	bool bIsCooldownActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Fire Settings")
	bool bPendingFire;	// Bu bayrak sayesinde, cooldown süresince FirePressed çağrısı yapılsa bile atış yapılmayacak. Böylece hızlı bas çek işlemlerinde cooldown süresi boyunca yeniden atış yapılmasının önüne geçilmiş olur.

	UPROPERTY(VisibleAnywhere, Category = "Weapon | Fire Settings")
	bool bCanFire; // Silahın ateş edip edemeyeceğini kontrol eder

	UPROPERTY(EditAnywhere, Category = "Fire Settings | Burst Mode")
	int32 BurstCount;
	
	UPROPERTY(VisibleAnywhere, Category = "Fire Settings")
	int32 BurstShotsRemaining;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Settings")
	TArray<EFireMode> SupportedFireModes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire Settings")
	int32 CurrentFireModeIndex;

	FTimerHandle FireRateTimerHandle; // Ateş hızını ayarlayan zamanlayıcı
	FTimerHandle CooldownTimerHandle; // Cooldown süresini yönetmek için kullanılır.

	UPROPERTY(EditAnywhere, Category = "Fire Mode")
	FString SingleShotName;

	UPROPERTY(EditAnywhere, Category = "Fire Mode")
	FString BurstFireName;

	UPROPERTY(EditAnywhere, Category = "Fire Mode")
	FString AutomaticName;
	
};