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

