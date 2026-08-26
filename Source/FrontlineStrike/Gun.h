// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SceneComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Gun.generated.h"

class AFrontlineStrikeCharacter;

UCLASS()
class FRONTLINESTRIKE_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere)
	AController* OwnerContuoller;

	UPROPERTY(VisibleAnywhere)
	AFrontlineStrikeCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	float MaxRange = 10000.0f;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* MuzzleFlashParticleSystem;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ImpactParticleSystem;

	UPROPERTY(EditAnywhere)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere)
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(VisibleAnywhere)
	bool CanShoot = true;

	UPROPERTY(EditAnywhere)
	float FireInterval = 0.15f;

	FTimerHandle ShootCooldownTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float RecoilPitch = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float RecoilYaw = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float AimRecoilPitch = 1.6f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float AimRecoilYaw = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float RecoilPitchOffset = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float RecoilReturnSpeed = 3.0f;

	UPROPERTY(VisibleAnywhere, Category = "Recoil")
	float CurrentRecoilPitch = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Recoil")
	float CurrentRecoilYaw = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Spread")
	float BaseSpreadAngle = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Spread")
	float AimSpreadAngle = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Spread")
	float MaxSpread = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Spread")
	float SpreadIncreasePerShot = 0.5f; 

	UPROPERTY(EditAnywhere, Category = "Spread")
	float SpreadDecaySpeed = 6.0f;

	UPROPERTY(VisibleAnywhere, Category = "Spread")
	float CurrentSpread = 0.0f;        
		
public:

	void PullTrigger();

	void OnFireCooldownEnd();

	void ApplyRecoil();
};
