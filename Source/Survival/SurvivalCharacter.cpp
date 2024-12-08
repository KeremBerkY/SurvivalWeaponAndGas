// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalCharacter.h"
// #include "WeaponPickupSystem/WeaponMaster.h"
// #include "WeaponPickupSystem/WeaponMaster.h"
// #include "WeaponPickupSystem/BasePickup.h"
// #include "WeaponPickupSystem/WeaponPickup.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/SphereComponent.h"
#include "WeaponPickupSystem/PickupSystem/BasePickup.h"
#include "AbilitySystemComponent.h"
#include "WeaponPickupSystem/Character/CharacterAnimInstance.h"
#include "WeaponPickupSystem/Character/CharacterStateComponent.h"
#include "WeaponPickupSystem/Character/WeaponInventory.h"
#include "WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "WeaponPickupSystem/Character/GAS/Abilities/CharacterGameplayAbility.h"
#include "WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"
#include "WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"
#include "WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival//WeaponPickupSystem/Character/GAS/Abilities/AbilityInputs/GASAbilityInputID.h"
#include "WeaponPickupSystem/Character/Components/GASEnhancedInputComponent.h"
#include "WeaponPickupSystem/Character/Components/ResourceComponent.h"
#include "WeaponPickupSystem/Character/PlayerStates/CharacterPlayerState.h"
#include "WeaponPickupSystem/Data/CharacterClassInfo.h"
#include "WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"
#include "WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "WeaponPickupSystem/UserInterface/ResourceWidget/ResourceWidget.h"
#include "WeaponPickupSystem/UserInterface/Interaction/InteractionWidget.h"


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
	PickupSphere->SetSphereRadius(200.f);  // Etkileşim yarıçapı
	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ASurvivalCharacter::OnPickupOverlapBegin);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &ASurvivalCharacter::OnPickupOverlapEnd);

	WeaponInventoryComponent = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventoryComponent"));
	CharacterStateComponent = CreateDefaultSubobject<UCharacterStateComponent>(TEXT("CharacterStateComponent"));
	CharacterWeaponComponent = CreateDefaultSubobject<UCharacterWeaponComponent>(TEXT("CharacterWeaponComponent"));
	GASEnhancedInputComponent = CreateDefaultSubobject<UGASEnhancedInputComponent>(TEXT("GASEnhancedInputComponent"));
	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("HealthComponent"));
	
}

void ASurvivalCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		InitAbilityActorInfo();
	}
}

void ASurvivalCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
	// BindAbilityInput();
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
//
// void ASurvivalCharacter::BindAbilityInput()
// {
// 	if (!CharacterAbilitySystemComponent)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("CharacterAbilitySystemComponent is null. Did you forget to initialize it in the constructor?"));
// 	}
// 	if (!InputComponent)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("InputComponent is null. Ensure InputComponent is initialized in BeginPlay or earlier."));
// 	}
// 	
// 	if (CharacterAbilitySystemComponent && InputComponent)
// 	{
// 		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", FTopLevelAssetPath(TEXT("/Script/Survival.EGASAbilityInputID")),
// 			static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
//
// 		CharacterAbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
//
// 		UE_LOG(LogTemp, Log, TEXT("Ability inputs successfully bound!"));
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Failed to bind ability inputs! Either CharacterAbilitySystemComponent or InputComponent is null."));
// 	}
// }

void ASurvivalCharacter::InitializeResourceComponent()
{
	UResourceWidget* ResourceWidget = HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetHealthWidget();

	if (ResourceWidget && !ResourceWidget->IsInViewport())
	{
		ResourceWidget->AddToViewport();

		ResourceComponent->OnHealthChanged.AddDynamic(ResourceWidget, &UResourceWidget::UpdateHealthBar);
		ResourceComponent->OnManaChanged.AddDynamic(ResourceWidget, &UResourceWidget::UpdateManaBar);
		ResourceComponent->OnStaminaChanged.AddDynamic(ResourceWidget, &UResourceWidget::UpdateStaminaBar);

		CharacterAbilitySystemComponent->AbilityActorInfoSet(); // Kaldır test içindi!
	}
	
}


void ASurvivalCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	HUD = Cast<ASurvivalSystemHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	// HUD = Cast<ASurvivalSystemHUD>(GetSurvivalHUD());
	if (!HUD) return;
	
	HUD->OnHUDReady.AddDynamic(this, &ASurvivalCharacter::InitializeResourceComponent);
	
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);

	GetCharacterWeaponComponent()->SetCurrentWeapon(nullptr);
	CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	
	//Add Input Mapping Context
	// if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	// {
	// 	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	// 	{
	// 		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	// 	}
	// }

	WeaponInventoryComponent = NewObject<UWeaponInventory>(this);

}


void ASurvivalCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	CheckPickupInterval(DeltaSeconds);
	
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


//////////////////////////////////////////////////////////////////////////
// Input

// void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	// Set up action bindings
// 	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
// 		// Jumping
// 		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
// 		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
// 		// Moving
// 		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Move);
// 		// Looking
// 		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Look);
// 		// Interact
// 		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Interact);
// 		// Fire
// 		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ASurvivalCharacter::FirePressed);
// 		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::FireReleased);
// 		// Reload
// 		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Reload);
// 		// Drop	
// 		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Drop);
// 		// Raycast Switch
// 		EnhancedInputComponent->BindAction(RaycastAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::RaycastWeaponSwitch);
// 		// Projectile Switch
// 		EnhancedInputComponent->BindAction(ProjectileAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::ProjectileWeaponSwitch);
// 		// Melee Switch
// 		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::MeleeWeaponSwitch);
// 		// Toggle Fire Mode
// 		EnhancedInputComponent->BindAction(ToggleFireModeAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::ToggleWeaponFireMode);
// 		// Sprinting
// 		// EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::StartSprinting);
// 		// EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::StopSrinting);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
// 	}
// 	
// 	// BindAbilityInput();
// }

// void ASurvivalCharacter::Move(const FInputActionValue& Value) // Updated! 
// {
// 	FVector2D MovementVector = Value.Get<FVector2D>();
//
// 	if (Controller != nullptr)
// 	{
// 		const FRotator Rotation = Controller->GetControlRotation();
// 		const FRotator YawRotation(0, Rotation.Yaw, 0);
// 		
// 		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
// 		
// 		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
// 		
// 		AddMovementInput(ForwardDirection, MovementVector.Y);
// 		AddMovementInput(RightDirection, MovementVector.X);
// 		
// 	}
// }

//
// void ASurvivalCharacter::StartSprinting()
// {
// 	// if (GetCharacterAbilitySystemComponent())
// 	// {
// 	// 	FGameplayTagContainer AbilityTags;
// 	// 	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Movement.Sprint")));
// 	// 	
// 	// 	GetCharacterAbilitySystemComponent()->TryActivateAbilitiesByTag(AbilityTags);
// 	// }
// }
//
// void ASurvivalCharacter::StopSprinting()
// {
// 	
// }

// void ASurvivalCharacter::Look(const FInputActionValue& Value) // Updated!
// {
// 	// input is a Vector2D
// 	FVector2D LookAxisVector = Value.Get<FVector2D>();
//
// 	if (Controller != nullptr)
// 	{
// 		// add yaw and pitch input to controller
// 		AddControllerYawInput(LookAxisVector.X);
// 		AddControllerPitchInput(LookAxisVector.Y);
// 	}
// }


void ASurvivalCharacter::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABasePickup* Pickup = Cast<ABasePickup>(OtherActor);
	if (Pickup)
	{
		UpdateCurrentPickup(Pickup);
	}
}

void ASurvivalCharacter::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABasePickup* Pickup = Cast<ABasePickup>(OtherActor);
	if (Pickup && Pickup == CurrentPickup)
	{
		UpdateCurrentPickup(nullptr);
	}
}

void ASurvivalCharacter::UpdateCurrentPickup(ABasePickup* NewPickup)
{
	if (CurrentPickup)
	{
		CurrentPickup->SetCustomDepth(false);
	}

	CurrentPickup = NewPickup;
	SetCurrentPickup(NewPickup);

	if (CurrentPickup)
	{
		CurrentPickup->SetCustomDepth(true);
		
		if (HUD)
		{
			HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetInteractionWidget()->UpdateWidget(&CurrentPickup->InteractableData);
			HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetInteractionWidget()->ShowWidget();
		}
	}
	else if (HUD)
	{
		HUD->GetMainHUDWidget()->GetGameHUDWidget()->GetInteractionWidget()->HideWidget();
	}
	
}

void ASurvivalCharacter::CheckPickupInterval(float DeltaTime)
{
	static float TimeAccumulator = 0.f;
	TimeAccumulator += DeltaTime;
	if (TimeAccumulator >= 0.5f)
	{
		CheckNearbyPickups();
		TimeAccumulator = 0.f;
	}
}

void ASurvivalCharacter::CheckNearbyPickups()
{
	TArray<AActor*> OverlappingActors;
	PickupSphere->GetOverlappingActors(OverlappingActors, ABasePickup::StaticClass());

	float NearestDistance = TNumericLimits<float>::Max();
	ABasePickup* NearestPickup = nullptr;

	for (AActor* Actor : OverlappingActors)
	{
		ABasePickup* Pickup = Cast<ABasePickup>(Actor);
		if (Pickup)
		{
			float Distance = (Pickup->GetActorLocation() - GetActorLocation()).Size();
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestPickup = Pickup;
			}
		}
	}
}

//
// void ASurvivalCharacter::Interact()  // Updated! Made with Ability
// {
// 	if (CurrentPickup)
// 	{
// 		IInteractionInterface* Interactable = Cast<IInteractionInterface>(CurrentPickup);
// 		if (Interactable)
// 		{
// 			Interactable->Interact(this);
// 			CurrentPickup = nullptr;
// 		}
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("No pickup available to interact with."));
// 	}
// }

// void ASurvivalCharacter::Drop()  // Updated! Made with Ability // TODO: Bu Drop(), CharacterWeaponComponent::WeaponDrop(this) // 
// {
// 	// if (CurrentWeapon)
// 	// {
// 	// 	AWeaponBase* WeaponDrop = Cast<AWeaponBase>(CurrentWeapon);
// 	// 	if (WeaponDrop)
// 	// 	{
// 	// 		WeaponDrop->DropWeapon(this); // TODO: Bu CharacterWeaponComponentda yine çağrılacak AWaponBase'nin üstüne düşen sorumluluk.
// 	// 		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 	// 		CurrentWeapon = nullptr;
// 	// 		CharacterAnimInstance->UpdateWeaponType(CurrentWeapon);
// 	// 	}
// 	// }
// 	if (CharacterWeaponComponent)
// 	{
// 		CharacterWeaponComponent->DropWeapon(this);
// 	}
// }

// void ASurvivalCharacter::RaycastWeaponSwitch()
// {
// 	if (CanSwitchWeapon())
// 	{
// 		WeaponInventoryComponent->SwapToBackWeapon(CharacterWeaponComponent->GetCurrentWeapon(), this, EWeaponCategories::EWC_RaycastWeapons);
// 		UpdateLastSwitchTime();
// 	}
// }
//
// void ASurvivalCharacter::ProjectileWeaponSwitch()
// {
// 	if (CanSwitchWeapon())
// 	{
// 		WeaponInventoryComponent->SwapToBackWeapon(CharacterWeaponComponent->GetCurrentWeapon(), this, EWeaponCategories::EWC_ProjectileWeapons);
// 		UpdateLastSwitchTime();
// 	}
// }
//
// void ASurvivalCharacter::MeleeWeaponSwitch()
// {
// 	if (CanSwitchWeapon())
// 	{
// 		WeaponInventoryComponent->SwapToBackWeapon(CharacterWeaponComponent->GetCurrentWeapon(), this, EWeaponCategories::EWC_MeleeWeapons);
// 		UpdateLastSwitchTime();
// 	}
// }
//
// bool ASurvivalCharacter::CanSwitchWeapon() const
// {
// 	return (GetWorld()->TimeSeconds - LastSwitchTime) >= WeaponSwitchCooldown;
// }
//
// void ASurvivalCharacter::UpdateLastSwitchTime()
// {
// 	LastSwitchTime = GetWorld()->TimeSeconds;
// }

// void ASurvivalCharacter::FirePressed() // Updated! Made with Ability
// {
// 	
// 	if (CharacterWeaponComponent->GetCurrentWeapon() && CharacterWeaponComponent->GetCurrentWeapon()->CanFire())
// 	{
// 		CharacterWeaponComponent->GetCurrentWeapon()->FireHandle(true);
// 	}
// 	else if (!CharacterWeaponComponent->GetCurrentWeapon())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("You have no weapon!"));
// 	}
// }
//
// void ASurvivalCharacter::FireReleased() // Updated! Made with Ability 
// {
// 	if (CharacterWeaponComponent->GetCurrentWeapon())
// 	{
// 		CharacterWeaponComponent->GetCurrentWeapon()->FireHandle(false);
// 	}
// }
//
// void ASurvivalCharacter::Reload()
// {
// 	if (CharacterWeaponComponent->GetCurrentWeapon())
// 	{
// 		CharacterWeaponComponent->GetCurrentWeapon()->Reload();
// 	}
// }
//
// void ASurvivalCharacter::ToggleWeaponFireMode()
// {
// 	if (CharacterWeaponComponent->GetCurrentWeapon())
// 	{
// 		CharacterWeaponComponent->GetCurrentWeapon()->ToggleFireMode();
// 	}
// }

/* Bu kısmı buradan taşı ! */
void ASurvivalCharacter::PlayerTotalCoin() const
{
	UE_LOG(LogTemp, Warning, TEXT("Player Coin: %d"), TotalCoin);
}




