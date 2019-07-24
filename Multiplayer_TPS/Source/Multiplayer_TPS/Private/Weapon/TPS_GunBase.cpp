// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_GunBase.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
ATPS_GunBase::ATPS_GunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
}

// Called when the game starts or when spawned
void ATPS_GunBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPS_GunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

