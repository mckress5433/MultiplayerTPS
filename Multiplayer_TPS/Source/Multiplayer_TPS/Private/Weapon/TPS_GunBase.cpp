// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_GunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include <UnrealMathUtility.h>
#include "Player/Public/TPS_Character.h"


// Sets default values
ATPS_GunBase::ATPS_GunBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
}

// Called when the game starts or when spawned
void ATPS_GunBase::BeginPlay()
{
	Super::BeginPlay();
	BulletCount = MagazineSize;
	BurstFireCount = MaxBurst;
	TimeBetweenShots = 60.0f / RateOfFire;
}

void ATPS_GunBase::BeginFire()
{
	if (!bIsFiring)
	{
		bIsFiring = true;
		float firstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->GetTimeSeconds(), 0.0f);
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATPS_GunBase::Fire, TimeBetweenShots, true, firstDelay);
	}
}

void ATPS_GunBase::EndFire()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	if (bIsBurstFire) {
		BurstFireCount = MaxBurst;
		if (BulletCount > 0) {
			bCanFire = true;
		}
	}
}

void ATPS_GunBase::Reload()
{
	BulletCount = MagazineSize;
	bCanFire = true;
}

bool ATPS_GunBase::CanReload()
{
	return (MagazineSize != BulletCount);
}

void ATPS_GunBase::Fire()
{
	if (!bCanFire) return;

	ATPS_Character* gunOwner = Cast<ATPS_Character>(GetOwner());
	if (gunOwner) {
		gunOwner->PlayFiringMontage();
	}

	LastFireTime = GetWorld()->TimeSeconds;
	BulletCount--;

	if(bIsBurstFire) BurstFireCount--;
	
	if (BulletCount <= 0 || BurstFireCount <= 0) {
		bCanFire = false;
		EndFire();
	}
}

FName ATPS_GunBase::GetWeaponSocketName()
{
	return WeaponSocketName;
}

float ATPS_GunBase::GetBaseDamage()
{
	return BaseDamage;
}

float ATPS_GunBase::GetZoomedFOV()
{
	return ZoomedFOV;
}

float ATPS_GunBase::GetZoomInterpSpeed()
{
	return ZoomInterpSpeed;
}

float ATPS_GunBase::GetZoomedCameraLagSpeed()
{
	return ZoomedCameraLagSpeed;
}

bool ATPS_GunBase::GetCanZoom()
{
	return bCanZoom;
}

TSubclassOf<UDamageType> ATPS_GunBase::GetDamageType()
{
	return DamageType;
}

