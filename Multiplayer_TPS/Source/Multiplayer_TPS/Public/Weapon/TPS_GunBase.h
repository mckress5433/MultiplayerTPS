// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/EngineTypes.h>
#include "TPS_GunBase.generated.h"

class USkeletalMeshComponent;
class UCameraShake;


UCLASS()
class MULTIPLAYER_TPS_API ATPS_GunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPS_GunBase();

	void BeginFire();
	void EndFire();
	void Reload();
	bool CanReload();

	FName GetWeaponSocketName();

	float GetBaseDamage();
	
	float GetZoomedFOV();

	float GetZoomInterpSpeed();

	float GetZoomedCameraLagSpeed();

	bool GetCanZoom();


	TSubclassOf<UDamageType> GetDamageType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Fire();

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunMesh;

	//The character socket that the gun attaches to
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponSocketName	   = "RifleSocket";
	//The socket that represents the muzzle of the gun
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	FName MuzzleSocketName     = "Muzzle";
	//The socket that the left hand of the character will attach to
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	FName SecondHandSocketName = "SecondHandSocket";
	//Base damage caused by the weapon
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage		   = 10.0f;
	//Damage type of the weapon
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	float ZoomedFOV = 65.0f;
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
	float ZoomInterpSpeed = 20.0f;
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
	float ZoomedCameraLagSpeed = 50.0f;
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	bool bCanZoom = true;

	FTimerHandle FireTimerHandle;

	bool bCanFire = true;
	bool bIsFiring = false;
	//Number of Bullets in chamber
	int BulletCount;
	//Number of shots before reloading
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	int MagazineSize = 6;
	
	//Number of shots fired during burst
	int BurstFireCount;
	//Number of shots that can be fired in one burst
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	bool bIsBurstFire = false;
	//Number of shots that can be fired in one burst
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	int MaxBurst = 6;

	//Timestamp of last shot fired
	float LastFireTime = 0;
	//Amount of time between shots (calculated from RateOfFire)
	float TimeBetweenShots;
	//Number of shots per minute
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	float RateOfFire = 600;

	UPROPERTY(Category = WeaponEffects, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCameraShake> FiringCameraShake;

};
