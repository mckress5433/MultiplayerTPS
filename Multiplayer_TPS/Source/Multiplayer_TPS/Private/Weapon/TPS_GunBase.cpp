// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_GunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

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

FHitResult ATPS_GunBase::GetFiringHitResult(AActor* _gunOwner)
{
	FVector eyeLocation;
	FRotator eyeRotation;
	_gunOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

	FVector traceEnd = eyeLocation + (eyeRotation.Vector() * 100000);

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(_gunOwner);
	queryParams.AddIgnoredActor(this);
	queryParams.bTraceComplex = true;

	FHitResult hitInfo;

	GetWorld()->LineTraceSingleByChannel(hitInfo, eyeLocation, traceEnd, ECC_Visibility, queryParams);
	
	return hitInfo;
}

void ATPS_GunBase::PlayFiringEffects(FHitResult _hitInfo)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, GunMesh, MuzzleSocketName);
	}

	if (BulletTraceEffect)
	{
		FVector muzzleLocation = GunMesh->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* bulletTraceComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTraceEffect, muzzleLocation);
		if (bulletTraceComp)
		{
			FVector traceEndPoint = _hitInfo.bBlockingHit ? _hitInfo.ImpactPoint : _hitInfo.TraceEnd;
			bulletTraceComp->SetVectorParameter("BeamEnd", traceEndPoint);
		}
	}

	if (_hitInfo.bBlockingHit && ImpactEffect) 
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, _hitInfo.ImpactPoint, _hitInfo.ImpactNormal.Rotation());
	}
}

void ATPS_GunBase::Fire()
{
	AActor* gunOwner = GetOwner();
	if (gunOwner) {
	
		FHitResult hitInfo = GetFiringHitResult(gunOwner);

		if (hitInfo.bBlockingHit) 
		{
			//if blocking hit then process damage
			AActor* hitActor = hitInfo.GetActor();

			UGameplayStatics::ApplyPointDamage(
				hitActor,
				BaseDamage,
				hitInfo.ImpactNormal,
				hitInfo,
				gunOwner->GetInstigatorController(),
				gunOwner,
				DamageType
			);
		}

		PlayFiringEffects(hitInfo);
	}
}

// Called every frame
void ATPS_GunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

