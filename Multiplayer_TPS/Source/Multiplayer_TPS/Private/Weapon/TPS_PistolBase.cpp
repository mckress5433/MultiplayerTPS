// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_PistolBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <DrawDebugHelpers.h>
#include "IConsoleManager.h"
#include <PhysicalMaterials/PhysicalMaterial.h>
#include "Multiplayer_TPS.h"

static int32 DebugWeaponDrawing = 0;
static FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("TPS.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw debug lines for weapons"),
	ECVF_Cheat
);

void ATPS_PistolBase::Fire()
{
	if (!bCanFire) return;

	Super::Fire();

	AActor* gunOwner = GetOwner();
	if (gunOwner) 
	{

		FHitResult hitInfo = GetFiringHitResult(gunOwner);
		
		EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitInfo.PhysMaterial.Get());
		
		if (hitInfo.bBlockingHit) 
		{
			float actualDamage = BaseDamage;
			if (surfaceType == SURFACE_FLESHVULNERABLE) actualDamage *= 2.0f;

			//if blocking hit then process damage
			AActor* hitActor = hitInfo.GetActor();

			UGameplayStatics::ApplyPointDamage
			(
				hitActor,
				actualDamage,
				hitInfo.ImpactNormal,
				hitInfo,
				gunOwner->GetInstigatorController(),
				gunOwner,
				DamageType
			);
		}

		PlayFiringEffects(hitInfo, surfaceType);
	}
}

FHitResult ATPS_PistolBase::GetFiringHitResult(AActor* _gunOwner)
{
	FVector eyeLocation;
	FRotator eyeRotation;
	_gunOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);
	
	FVector traceEnd = eyeLocation + (eyeRotation.Vector() * 100000);
	
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(_gunOwner);
	queryParams.AddIgnoredActor(this);
	queryParams.bReturnPhysicalMaterial = true;
	queryParams.bTraceComplex = true;
	
	FHitResult hitInfo;
	
	GetWorld()->LineTraceSingleByChannel(hitInfo, eyeLocation, traceEnd, COLLISION_WEAPON, queryParams);
		
	if (DebugWeaponDrawing > 0) {
		DrawDebugLine(GetWorld(), eyeLocation, traceEnd, FColor::Blue, false, 1.0f, 0, 1.0f);
		if (hitInfo.bBlockingHit) {
			DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 10.0f, 10.0f, FColor::Green, false, 1.0f, 0, 1.0f);
		}
	}

	return hitInfo;
}

void ATPS_PistolBase::PlayFiringEffects_Implementation(FHitResult _hitInfo, EPhysicalSurface _surfaceType)
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
	
	if (_hitInfo.bBlockingHit)
	{
		UParticleSystem* selectedImpactEffect = nullptr;

		switch (_surfaceType) 
		{
			case SURFACE_FLESHDEFAULT:
			case SURFACE_FLESHVULNERABLE:
				selectedImpactEffect = ImpactEffect_Flesh;
				break;
			default:
				selectedImpactEffect = ImpactEffect_Default;
		}

		if (selectedImpactEffect) 
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectedImpactEffect, _hitInfo.ImpactPoint, _hitInfo.ImpactNormal.Rotation());
		}
		
	}

	APawn* GunOwner = Cast<APawn>(GetOwner());
	if (GunOwner) {
		APlayerController* PC = Cast<APlayerController>(GunOwner->GetController());
		if (PC) {
			PC->ClientPlayCameraShake(FiringCameraShake);
		}
	}
}
