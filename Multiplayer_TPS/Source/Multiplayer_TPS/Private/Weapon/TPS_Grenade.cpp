// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Grenade.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ATPS_Grenade::ATPS_Grenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GrenadeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GrenadeMesh"));
	RootComponent = GrenadeMesh;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 20000.0f;
	ProjectileMovementComp->MaxSpeed = 2000.0f;
	ProjectileMovementComp->bShouldBounce = true;
}

void ATPS_Grenade::PrimeGrenade()
{
	FTimerHandle detonationHandle;
	GetWorldTimerManager().SetTimer(detonationHandle, this, &ATPS_Grenade::Detonate, 1.0f, false, 1.0f);
}

// Called when the game starts or when spawned
void ATPS_Grenade::BeginPlay()
{
	Super::BeginPlay();

}

void ATPS_Grenade::Detonate()
{
	if (ExplosionEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}

	Destroy();
}

// Called every frame
void ATPS_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

