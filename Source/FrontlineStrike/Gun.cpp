// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Kismet/GameplayStatics.h"
#include "FrontlineStrikeCharacter.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(SceneComp);

	MuzzleFlashParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleFlashParticle"));
	MuzzleFlashParticleSystem->SetupAttachment(SceneComp);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	MuzzleFlashParticleSystem->Deactivate();

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnerCharacter)
	{
		float DeltaPitch = CurrentRecoilPitch;
		float DeltaYaw = CurrentRecoilYaw;

		CurrentRecoilPitch = FMath::FInterpTo(CurrentRecoilPitch, 0, DeltaTime, RecoilReturnSpeed);
		CurrentRecoilYaw = FMath::FInterpTo(CurrentRecoilYaw, 0, DeltaTime, RecoilReturnSpeed);

		OwnerCharacter->AddControllerPitchInput(CurrentRecoilPitch - DeltaPitch);
		OwnerCharacter->AddControllerYawInput(CurrentRecoilYaw - DeltaYaw);

		CurrentSpread = FMath::FInterpTo(CurrentSpread, 0.f, DeltaTime, SpreadDecaySpeed);
	}
}

void AGun::PullTrigger()
{
	MuzzleFlashParticleSystem->Activate(true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation());
	if (OwnerContuoller)
	{
		FVector ViewPointLocation;
		FRotator ViewPointRotation;
		OwnerContuoller->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

		float EffectiveSpread = OwnerCharacter->IsAiming ? AimSpreadAngle : BaseSpreadAngle;

		CurrentSpread = FMath::Min(CurrentSpread + SpreadIncreasePerShot, MaxSpread);

		EffectiveSpread += CurrentSpread;

		FVector Direction = ViewPointRotation.Vector();

		FVector Right = FRotationMatrix(ViewPointRotation).GetUnitAxis(EAxis::Y);

		FVector Up = FRotationMatrix(ViewPointRotation).GetUnitAxis(EAxis::Z);

		Direction = Direction.RotateAngleAxis(FMath::FRandRange(-EffectiveSpread, EffectiveSpread), Right);

		Direction = Direction.RotateAngleAxis(FMath::FRandRange(-EffectiveSpread, EffectiveSpread), Up);

		FVector EndLocation = ViewPointLocation + Direction.GetSafeNormal() * MaxRange;

		//碰撞结果
		FHitResult HitResult;

		//碰撞查询参数
		FCollisionQueryParams Params;

		//添加被忽略者
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());
		bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPointLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);

		CanShoot = false;

		if (IsHit)
		{
			//生成命中特效
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticleSystem, 
				HitResult.ImpactPoint, HitResult.ImpactPoint.Rotation());

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);

			AActor* HitActor = HitResult.GetActor();
			if (HitActor) 
			{
				UGameplayStatics::ApplyDamage(HitActor, Damage, OwnerContuoller, this, UDamageType::StaticClass());
			}
		}

		GetWorld()->GetTimerManager().SetTimer(
			ShootCooldownTimerHandle,
			this,
			&AGun::OnFireCooldownEnd,
			FireInterval,
			false);
		ApplyRecoil();
	}
}

void AGun::OnFireCooldownEnd()
{
	CanShoot = true;
}

void AGun::ApplyRecoil()
{
	if (OwnerCharacter)
	{
		bool IsAiming = OwnerCharacter->IsAiming;

		float Pitch = IsAiming ? AimRecoilPitch : RecoilPitch;
		float Yaw = IsAiming ? AimRecoilYaw : RecoilYaw;
		float PitchOffset = IsAiming ? RecoilPitchOffset : 0.0f;

		CurrentRecoilPitch += RecoilPitch + FMath::FRandRange(0.0f, PitchOffset);
		CurrentRecoilYaw = FMath::FRandRange(-Yaw, Yaw);
	}
}

