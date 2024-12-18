// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireModes/FireModeConfig.h"
#include "GameFramework/Actor.h"
#include "Survival/WeaponPickupSystem/UserInterface/Crosshair/CrosshairBaseWidget.h"
#include "WeaponBase.generated.h"

class UImage;
class IFireMode;
class AWeaponPickup;
class ASurvivalCharacter;

UENUM(BlueprintType)
enum class EWeaponCategories : uint8
{
	EWC_RaycastWeapons UMETA(DisplayName = "RaycastWeapons"),
	EWC_ProjectileWeapons UMETA(DisplayName = "ProjectileWeapons"),
	EWC_MeleeWeapons UMETA(DisplayName = "MeleeWeapons"),
	EWC_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	// Raycast Silahlar
	Ewt_Unarmed UMETA(DisplayName = "Unarmed"),
	Ewt_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	Ewt_Pistol UMETA(DisplayName = "Pistol"),
	Ewt_Shotgun UMETA(DisplayName = "Shotgun"),

	// Projectile Silahlar
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),
	Ewt_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),

	// Melee Silahlar
	EWT_Sword UMETA(DisplayName = "Sword"),
	EWT_Axe UMETA(DisplayName = "Axe"),

	// Diğer türler için
	EWT_None UMETA(DisplayName = "None"),
	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FWeaponAttributes // AmmoCapacity total Ammo buna bir üst limitte koy bir yere kadar alınabilsin. AmmoMagazine de şarjör miktarı. AmmoMagazine biterse AmmoCapacityden AmmoMagazine kadar veya kalan kadar alsın ve yenilemeleri yap.
{
	GENERATED_USTRUCT_BODY()

	FWeaponAttributes() : WeaponName(TEXT("")), WeaponCategories(EWeaponCategories::EWC_MAX),
	                      Damage(0), AmmoCapacity(0), AmmoInMagazine(0), CurrentAmmo(0), WeaponImage(nullptr)
	{}; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	EWeaponCategories WeaponCategories;

	// Crosshair comment line made \\
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	// TSubclassOf<UCrosshairBaseWidget> CrosshairWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 AmmoCapacity; // Taşınabilir maksimum mermi miktarı, yani karakterin üzerinde bulundurabileceği toplam mermi kapasitesi.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 AmmoInMagazine; // Şarjörün tam kapasitesi, yani her reload işleminde şarjöre dolan mermi miktarı

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmo; // Şarjörde şu anda bulunan mermi miktarı.

	// // TODO: SocketName'leri Silahı yaratırken tanımla!! Daha Sonra GetSocketName() Fonksiyonundan kullanılan socketlere ulaş
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	// FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UTexture2D* WeaponImage;
	
	// Getter ve setter fonksiyonları
	FORCEINLINE FString GetWeaponName() const { return WeaponName; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE int32 GetAmmoCapacity() const { return AmmoCapacity; }
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }
	FORCEINLINE int32 GetAmmoMagazine() const { return AmmoInMagazine; }
	FORCEINLINE UTexture2D* GetWeaponImage() const { return WeaponImage; }

	FORCEINLINE void SetCurrentAmmo(int32 NewAmmo) { CurrentAmmo = NewAmmo; }
};

USTRUCT(BlueprintType)
struct FRecoilSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float BaseRecoilAmount = 1.0f; // Genel geri tepme kuvveti

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float PitchMultiplier = 1.0f; // Yükseklik (Pitch) çarpanı

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float YawMultiplier = 0.2f; // Yatay (Yaw) çarpanı

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float RandomRecoilRange = 0.5f; // Recoil için rastgele bir seed aralığı

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float RecoilCooldown = 0.1f; // Recoil soğuma süresi
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Recoil Settings")
	bool bCanApplyRecoil = true; // Recoil'in uygulanabilir olup olmadığını kontrol eder
	
	FTimerHandle RecoilCooldownTimerHandle;
};

USTRUCT(BlueprintType)
struct FDecalSettings
{
	GENERATED_USTRUCT_BODY()

	FDecalSettings() :
			DecalMaterial(nullptr),
			DecalSize(FVector(10.0f, 10.0f, 10.0f)),
			DecalLifeSpan(10.f) {} ;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UMaterialInterface* DecalMaterial; // Decal için materyal

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FVector DecalSize; // Decal boyutu

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float DecalLifeSpan; // Decal'in sahnede kalma süresi
	
};

UCLASS()
class SURVIVAL_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	FORCEINLINE AWeaponBase* GetCurrentWeapon() { return this; }
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE USceneComponent* GetMuzzleLocation() const { return MuzzleLocation; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE bool CanFire() const { return FireSettings.bCanFire; }
	FORCEINLINE FFireSettings& GetFireSettings() { return FireSettings; }
	FORCEINLINE TScriptInterface<IFireMode> GetCurrentFireMode() const { return CurrentFireMode; }
	FORCEINLINE EWeaponCategories GetWeaponCategory() const { return WeaponCategory; }
	FORCEINLINE TSubclassOf<AWeaponPickup> GetWeaponPickupClass() const { return  WeaponPickupClass; }
	FORCEINLINE FWeaponAttributes GetWeaponAttributes() const { return WeaponAttributes; }

	


	UFUNCTION()
	virtual void Fire();
	UFUNCTION()
	virtual void Reload();
	UFUNCTION()
	virtual void FinishReload();

	UFUNCTION()
	void ResetRecoil();
	UFUNCTION()
	void ResetCanFire();
	UFUNCTION()
	void FireHandle(bool bPressed);
	UFUNCTION()
	void ToggleFireMode();
	UFUNCTION()
	void ExecuteFire();
	UFUNCTION()
	void StartCooldown();

	


protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void PlayWeaponEffect();
	UFUNCTION()
	virtual void ApplyRecoilEffect();
	UFUNCTION()
	void InitializeFireModes();

	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	UParticleSystem* BaseEffect;
	
	UPROPERTY()
	TScriptInterface<IFireMode> CurrentFireMode;
	
	UPROPERTY()
	TMap<EFireMode, TScriptInterface<IFireMode>> FireModeMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EFireMode CurrentFireModeType = EFireMode::SingleShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Settings")
	TArray<EFireMode> SupportedFireModes;

	UPROPERTY(EditAnywhere, Category = "Weapon | Categories", meta = (AllowPrivateAccess = "true"))
	EWeaponCategories WeaponCategory;

	UPROPERTY(EditAnywhere, Category = "Weapon | Types", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category = "Weapon | Fire Modes", meta = (AllowPrivateAccess = "true"))
	EFireMode FireMode;
	
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FWeaponAttributes WeaponAttributes;

	UPROPERTY(EditAnywhere, Category = "Weapon | Recoil Settings", meta = (AllowPrivateAccess = "true"))
	FRecoilSettings RecoilSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponPickup> WeaponPickupClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Weapon | Fire Settings", meta = (AllowPrivateAccess = "true"))
	FFireSettings FireSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Decal Settings", meta = (AllowPrivateAccess = "true"))
	FDecalSettings DecalSettings;
	
private:
	UFUNCTION()
	void ResetWeaponPosition(FVector OriginalLocation);
	
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleLocation;
	
	UPROPERTY(EditAnywhere, Category  = "Weapon", meta = (AllowPrivateAccess = "true"))
	float DropDistance = 200.f;

	
	UPROPERTY()
	bool bIsReloading;

	FTimerHandle ReloadTimerHandle;
	
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float ReloadDuration = 2.0f;

};
