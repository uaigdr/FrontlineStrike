// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "Gun.h"

#include "FrontlineStrikeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM()
enum class MoveStates : uint8
{
	Idle,        // 原地静止
	Walk,        //普通行走
	Sprint,      //疾跑
	SlientWalk    //静步（慢走）
};

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AFrontlineStrikeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* MovementComp;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SlientWalkAction;

public:

	/** Constructor */
	AFrontlineStrikeCharacter();	

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void StartAim();
	void EndAim();

	void StartSprint();
	void EndSprint();

	void StartSlientWalk();
	void EndSlientWalk();

public:

	/** Called for Shooting input */
	void StartShoot();
	void EndShoot();

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(VisibleAnywhere)
	AGun* Gun;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(BlueprintReadOnly)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere)
	bool IsAiming = false;

	UPROPERTY(VisibleAnywhere)
	float StartTargetLength;

	UPROPERTY(EditAnywhere)
	float EndTargetLength = 100.0f;

	UPROPERTY(VisibleAnywhere)
	float StartFOV;

	UPROPERTY(EditAnywhere)
	float EndFOV = 70.0f;

	UPROPERTY(EditAnywhere)
	float TargetInterpSpeed = 20.0f;

	UPROPERTY(EditAnywhere)
	float FOVInterpSpeed = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float NormalWalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintWalkSpeed = 700.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SlientWalkSpeed = 200.f;

	UPROPERTY(BlueprintReadOnly)
	bool IsShooting = false;

	UPROPERTY(VisibleAnywhere)
	MoveStates MoveState = MoveStates::Idle;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* FireReactMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireAnimPlayRate = 1.33f;

public:

	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
		class AController* InstigatedBy, AActor* DamageCauser);

	void UpdateHUD();
};

