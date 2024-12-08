// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/SurvivalCharacter.h"
#include "UObject/Object.h"
#include "WeaponAttachmentManager.generated.h"

class AWeaponBase;

USTRUCT(BlueprintType)
struct FWeaponSockets
{
	GENERATED_USTRUCT_BODY()

	FWeaponSockets()
	: WeaponSocket(TEXT("WeaponSocket")),
	  RaycastSocket(TEXT("RaycastSocket")),
	  RaycastPistolSocket(TEXT("RaycastPistolSocket")),
	  ProjectileSocket(TEXT("ProjectileSocket")),
	  MeleeSocket(TEXT("MeleeSocket"))
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Socket", meta = (AllowPrivateAccess = "true"))
	FName WeaponSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Socket", meta = (AllowPrivateAccess = "true"))
	FName RaycastSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Socket", meta = (AllowPrivateAccess = "true"))
	FName RaycastPistolSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Socket", meta = (AllowPrivateAccess = "true"))
	FName ProjectileSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Socket", meta = (AllowPrivateAccess = "true"))
	FName MeleeSocket;
};

UCLASS()
class SURVIVAL_API UWeaponAttachmentManager : public UObject
{
	GENERATED_BODY()

public:
	FName GetWeaponSocketName(UCharacterWeaponComponent* WeaponComponent, AWeaponBase* Weapon);
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName, AWeaponBase* Weapon);
	void AttachWeaponToSocket(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter, FName CustomSocketName);
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FWeaponSockets WeaponSockets;
protected:

private:

	FName DefaultSocketName = NAME_None;
};
