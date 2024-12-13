// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "FireModes/FireModeFactory.h"
#include "FireModes/SingleShotFireMode.h"



AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponMesh);

	SupportedFireModes.Add(EFireMode::SingleShot);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeFireModes();
}

void AWeaponBase::InitializeFireModes()
{
	FireModeMap.Empty();
	
	FireModeMap = UFireModeFactory::CreateFireModes(this, SupportedFireModes);
	
	if (FireModeMap.Contains(EFireMode::SingleShot))
	{
		CurrentFireMode = FireModeMap[EFireMode::SingleShot];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No initial fire mode set."));
	}
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::FireHandle(bool bPressed)
{
	if (!FireSettings.bCanFire)
	{
		return;
	}

	if (WeaponAttributes.GetCurrentAmmo() <= 0)
	{
		if (CurrentFireMode.GetObject())
		{
			CurrentFireMode->StopFire(this); // Mermi bittiğinde ateşi durdur
		}
		return;
	}
	
	if (bPressed)
	{
		if (CurrentFireMode.GetObject() && CanFire())
		{
			CurrentFireMode->StartFire(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No valid fire mode or cannot fire."));
		}
	}
	else
	{
		if (CurrentFireMode.GetObject())
		{
			CurrentFireMode->StopFire(this);
		}
	}
}

void AWeaponBase::Fire()
{
	if (WeaponAttributes.GetCurrentAmmo() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No bullets, no firing possible."));
		return;
	}
	
	WeaponAttributes.SetCurrentAmmo(WeaponAttributes.GetCurrentAmmo() - 1);
	UE_LOG(LogTemp, Warning, TEXT("Shot fired! Remaining bullets: %d"), WeaponAttributes.GetCurrentAmmo());
}

void AWeaponBase::Reload()
{
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloading is already in progress."));
		return;
	}
	
	if (WeaponAttributes.GetCurrentAmmo() >= WeaponAttributes.GetAmmoMagazine())
	{
		UE_LOG(LogTemp, Warning, TEXT("The magazine is already loaded!"));
		return;
	}

	if (WeaponAttributes.GetAmmoCapacity() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough ammo!"));
		return;
	}

	bIsReloading = true;

	int32 AmmoNeeded = WeaponAttributes.GetAmmoMagazine() - WeaponAttributes.GetCurrentAmmo();
	int32 AmmoToReload = FMath::Min(AmmoNeeded, WeaponAttributes.GetAmmoCapacity());

	WeaponAttributes.CurrentAmmo += AmmoToReload;
	WeaponAttributes.AmmoCapacity -= AmmoToReload;

	UE_LOG(LogTemp, Log, TEXT("Reloaded: Magazine: %d, Total Ammo: %d"), WeaponAttributes.CurrentAmmo, WeaponAttributes.AmmoCapacity);
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::FinishReload, ReloadDuration, false);
}

void AWeaponBase::FinishReload()
{
	bIsReloading = false;
	UE_LOG(LogTemp, Log, TEXT("Reload complete."));
}


void AWeaponBase::ToggleFireMode()
{
	if (FireModeMap.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("FireModeMap is empty. Cannot toggle fire mode."));
		return;
	}
	
	if (FireModeMap.Num() > 1)
	{
		int32 CurrentIndex = static_cast<int32>(CurrentFireModeType);
		int32 NextIndex = (CurrentIndex + 1) % FireModeMap.Num();
		
		CurrentFireModeType = static_cast<EFireMode>(NextIndex);
		
		if (FireModeMap.Contains(CurrentFireModeType))
		{
			TScriptInterface<IFireMode> NewFireMode = FireModeMap[CurrentFireModeType];
			if (NewFireMode)
			{
				FString ModeName = NewFireMode->GetModeName();
				UE_LOG(LogTemp, Warning, TEXT("Fire mode changed to: %s"), *ModeName);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve new fire mode. NewFireMode is nullptr."));
			}
			
			CurrentFireMode = NewFireMode;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Only one fire mode available."));
	}
}

void AWeaponBase::StartCooldown()
{
	FireSettings.bCanFire = false;
	
	GetWorld()->GetTimerManager().SetTimer(
		FireSettings.CooldownTimerHandle,
		this,
		&AWeaponBase::ResetCanFire,
		FireSettings.FireRate,
		false
	);
}


void AWeaponBase::ExecuteFire()
{
	PlayWeaponEffect();
	ApplyRecoilEffect();
}

void AWeaponBase::ResetCanFire()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetCanFire() Called!! bCanFire = true"));
	FireSettings.bCanFire = true;
	GetWorld()->GetTimerManager().ClearTimer(FireSettings.CooldownTimerHandle);
}

void AWeaponBase::PlayWeaponEffect()
{
	if (BaseEffect)
	{
		// TODO: Burada hepsinde bulunacak olan bir efekti spawn edebilirsin...
	}
}

void AWeaponBase::ApplyRecoilEffect()
{
	if (!WeaponMesh || !RecoilSettings.bCanApplyRecoil) return;
	
	if (WeaponMesh)
	{
		float RecoilAmount = RecoilSettings.BaseRecoilAmount + FMath::FRandRange(-RecoilSettings.RandomRecoilRange, RecoilSettings.RandomRecoilRange);
		FVector RecoilOffset = FVector(-RecoilAmount * RecoilSettings.PitchMultiplier, RecoilAmount * RecoilSettings.YawMultiplier, 0.0f);

		FVector OriginalLocation = WeaponMesh->GetRelativeLocation();
		WeaponMesh->SetRelativeLocation(OriginalLocation + RecoilOffset);


		RecoilSettings.bCanApplyRecoil = false;
		GetWorld()->GetTimerManager().SetTimer(
			RecoilSettings.RecoilCooldownTimerHandle,
			this, &AWeaponBase::ResetRecoil,
			RecoilSettings.RecoilCooldown,
			false
			);
		
		ResetWeaponPosition(OriginalLocation);
	}
}

void AWeaponBase::ResetRecoil()
{
	RecoilSettings.bCanApplyRecoil = true;
}

void AWeaponBase::ResetWeaponPosition(FVector OriginalLocation)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, OriginalLocation]()
	{
		if (WeaponMesh)
		{
			WeaponMesh->SetRelativeLocation(OriginalLocation);
		}
	}, 0.1f, false); 
}
