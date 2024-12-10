// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "WeaponPickupSystem/Character/CharacterAnimInstance.h"
#include "WeaponPickupSystem/Character/CharacterStateComponent.h"
#include "WeaponPickupSystem/Character/Components/WeaponInventory.h"
#include "WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"
#include "WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "WeaponPickupSystem/Character/Components/GASEnhancedInputComponent.h"
#include "WeaponPickupSystem/Character/Components/PickupComponent.h"
#include "WeaponPickupSystem/Character/Components/ResourceComponent.h"
#include "WeaponPickupSystem/Character/PlayerStates/CharacterPlayerState.h"
#include "WeaponPickupSystem/Data/CharacterClassInfo.h"
#include "WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"
#include "WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "WeaponPickupSystem/UserInterface/ResourceWidget/ResourceWidget.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASurvivalCharacter

ASurvivalCharacter::ASurvivalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // Camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true;
	
	CharacterCameraComponent = CreateDefaultSubobject<UCharacterCameraComponent>(TEXT("CharacterCameraComponent"));
	CharacterCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach to the spring arm
	CharacterCameraComponent->bUsePawnControlRotation = false;

	// // Interact
	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(RootComponent);
	PickupSphere->SetSphereRadius(200.f);

	WeaponInventoryComponent = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventoryComponent"));
	CharacterStateComponent = CreateDefaultSubobject<UCharacterStateComponent>(TEXT("CharacterStateComponent")); // Kullanılmıyor! Tag ile yapılacak...
	CharacterWeaponComponent = CreateDefaultSubobject<UCharacterWeaponComponent>(TEXT("CharacterWeaponComponent"));
	GASEnhancedInputComponent = CreateDefaultSubobject<UGASEnhancedInputComponent>(TEXT("GASEnhancedInputComponent"));
	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("HealthComponent"));
	PickupComponent = CreateDefaultSubobject<UPickupComponent>(TEXT("PickupComponent"));
	
}

void ASurvivalCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilityActorInfo();
	
}

void ASurvivalCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}
	
void ASurvivalCharacter::InitAbilityActorInfo()
{
	if (ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>())
	{
		CharacterAbilitySystemComponent = CharacterPlayerState->GetCharacterAbilitySystemComponent();
		CharacterAttributes = CharacterPlayerState->GetCharacterAttributes();

		if (IsValid(CharacterAbilitySystemComponent))
		{
			CharacterAbilitySystemComponent->InitAbilityActorInfo(CharacterPlayerState, this);

			// Class Info is coming from the GameMode you only want to this one on the Server
			if (HasAuthority())
			{
				InitClassDefaults();
			}
		}
	}
}

void ASurvivalCharacter::InitClassDefaults()
{
	if (!CharacterTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Character Tag Selected In This Character %s"), *GetNameSafe(this));
	}
	else if (UCharacterClassInfo* ClassInfo = USurvivalAbilitySystemLibrary::GetCharacterClassDefaultInfo(this)) // if this is on the server
	{
		if (FCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag))
		{
			if (IsValid(CharacterAbilitySystemComponent))
			{
				CharacterAbilitySystemComponent->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
				CharacterAbilitySystemComponent->AddCharacterPassiveAbilities(SelectedClassInfo->StartingPassives);
				CharacterAbilitySystemComponent->InitializeDefaultAttributes(SelectedClassInfo->DefaultAttributes);
				CharacterAbilitySystemComponent->InitializeSpecialAttributes(SelectedClassInfo->SpecialAttributes);
				
				GetWorld()->GetTimerManager().SetTimer(
					InitializeDelayTimerHandle,
					this,
					&ASurvivalCharacter::HandleInitializeDelay,
					1.f
				);
				
			}
		}
	}
}

void ASurvivalCharacter::HandleInitializeDelay()
{
	InitializeResourceComponent();
	if (ResourceComponent && CharacterAbilitySystemComponent && CharacterAttributes)
	{
		ResourceComponent->InitializeWithGAS(CharacterAbilitySystemComponent, CharacterAttributes);
	}
}


void ASurvivalCharacter::InitializeResourceComponent()
{
	UResourceWidget* ResourceWidget = HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetHealthWidget();

	if (ResourceWidget && !ResourceWidget->IsInViewport())
	{
		ResourceWidget->AddToViewport();

		ResourceComponent->OnHealthChanged.AddDynamic(ResourceWidget, &UResourceWidget::UpdateHealthBar);
		ResourceComponent->OnManaChanged.AddDynamic(ResourceWidget, &UResourceWidget::UpdateManaBar);
		ResourceComponent->OnStaminaChanged.AddDynamic(ResourceWidget, &UResourceWidget::UpdateStaminaBar);

		// CharacterAbilitySystemComponent->AbilityActorInfoSet(); // Kaldır test içindi!
	}
}

void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	HUD = Cast<ASurvivalSystemHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!HUD) return;
	HUD->OnHUDReady.AddDynamic(this, &ASurvivalCharacter::InitializeResourceComponent);
	
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);

	GetCharacterWeaponComponent()->SetCurrentWeapon(nullptr);
	CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	
}


void ASurvivalCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

ASurvivalSystemHUD* ASurvivalCharacter::GetSurvivalHUD() const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		return Cast<ASurvivalSystemHUD>(PlayerController->GetHUD());
	}
	return nullptr;
}


