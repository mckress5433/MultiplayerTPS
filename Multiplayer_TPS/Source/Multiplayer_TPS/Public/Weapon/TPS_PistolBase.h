// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TPS_GunBase.h"
#include "TPS_PistolBase.generated.h"

class UParticleSystem;



UCLASS()
class MULTIPLAYER_TPS_API ATPS_PistolBase : public ATPS_GunBase
{
	GENERATED_BODY()

public:


protected:
	virtual void Fire() override;

	//Particle system used as muzzle flash when gun is fired
	UPROPERTY(Category = WeaponEffects, EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* MuzzleEffect;
	//Particle system used when projectile impacts with something
	UPROPERTY(Category = WeaponEffects, EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ImpactEffect_Default;
	//Particle system used when projectile impacts with flesh
	UPROPERTY(Category = WeaponEffects, EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ImpactEffect_Flesh;
	//Particle system used as bullet trace when weapon is fired
	UPROPERTY(Category = WeaponEffects, EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* BulletTraceEffect;



	virtual FHitResult GetFiringHitResult(AActor* _gunOwner);

	UFUNCTION(NetMulticast, Reliable)
	virtual void PlayFiringEffects(FHitResult _hitInfo, EPhysicalSurface _surfaceType);
	
};
