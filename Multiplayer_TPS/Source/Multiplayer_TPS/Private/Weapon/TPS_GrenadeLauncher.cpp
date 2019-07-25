// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_GrenadeLauncher.h"

void ATPS_GrenadeLauncher::Fire()
{
	AActor* gunOwner = GetOwner();
	if (gunOwner && ProjectileClass) {
		FVector eyeLocation;
		FRotator eyeRotation;
		gunOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

		FVector muzzleLocation = GunMesh->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATPS_Grenade* grenade = GetWorld()->SpawnActor<ATPS_Grenade>(ProjectileClass, muzzleLocation, eyeRotation);
		grenade->PrimeGrenade();
	}
}
