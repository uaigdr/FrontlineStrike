// Copyright Epic Games, Inc. All Rights Reserved.

#include "FrontlineStrikeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "FrontlineStrike.h"

#include "FrontlineStrikePlayerController.h"

AFrontlineStrikeCharacter::AFrontlineStrikeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

}

void AFrontlineStrikeCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	UpdateHUD();

	OnTakeAnyDamage.AddDynamic(this, &AFrontlineStrikeCharacter::OnDamageTaken);

	GetMesh()->HideBoneByName("weapon_r", EPhysBodyOp::PBO_None);

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	if (Gun)
	{
		Gun->SetOwner(this);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->OwnerContuoller = GetController();
	}

	StartTargetLength = CameraBoom->TargetArmLength;
	StartFOV = FollowCamera->FieldOfView;
}

void AFrontlineStrikeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 目标长度：瞄准时缩短到 EndTargetLength，松开后回到 StartTargetLength
	float TargetLength = IsAiming ? EndTargetLength : StartTargetLength;
	float Fov = IsAiming ? EndFOV : StartFOV;

	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, DeltaTime, TargetInterpSpeed);
	FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, Fov, DeltaTime, FOVInterpSpeed);
}

void AFrontlineStrikeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFrontlineStrikeCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AFrontlineStrikeCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrontlineStrikeCharacter::Look);

		// Shoot
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AFrontlineStrikeCharacter::Shoot);

		// Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AFrontlineStrikeCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AFrontlineStrikeCharacter::EndAim);
	}
	else
	{
		UE_LOG(LogFrontlineStrike, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFrontlineStrikeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AFrontlineStrikeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AFrontlineStrikeCharacter::StartAim()
{
	IsAiming = true;
}

void AFrontlineStrikeCharacter::EndAim()
{
	IsAiming = false;
}

void AFrontlineStrikeCharacter::Shoot()
{
	if (Gun) 
	{ 
		Gun->PullTrigger(); 
	}
}

void AFrontlineStrikeCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AFrontlineStrikeCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AFrontlineStrikeCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AFrontlineStrikeCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AFrontlineStrikeCharacter::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsAlive)
	{
		Health = Health > Damage ? Health - Damage : 0;
		UpdateHUD();
		if (Health == 0)
		{
			IsAlive = false;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			DetachFromControllerPendingDestroy();
		}
	}
}

void AFrontlineStrikeCharacter::UpdateHUD()
{
	AFrontlineStrikePlayerController* PlayerController = Cast<AFrontlineStrikePlayerController>(GetController());

	if (PlayerController)
	{
		PlayerController->HUDWidget->SetHealthBarPercent(Health / MaxHealth);
	}
}



