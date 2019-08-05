// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_GunBase.h"
#include "Components/SkeletalMeshComponent.h"

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
	
}

void ATPS_GunBase::Fire()
{

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

