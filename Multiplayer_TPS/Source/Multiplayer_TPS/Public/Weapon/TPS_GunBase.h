// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS_GunBase.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;

UCLASS()
class MULTIPLAYER_TPS_API ATPS_GunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPS_GunBase();

	virtual void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunMesh;



	/************  Weapon Properties  ***********/

	//The character socket that the gun attaches to
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponSocketName = "RifleSocket";
	//The socket that the left hand of the character will attach to
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	FName SecondHandSocketName = "SecondHandSocket";
	//The socket that represents the muzzle of the gun
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	FName MuzzleSocketName = "Muzzle";
	//Base damage caused by the weapon
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage = 10.0f;
	//Damage type of the weapon
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDamageType> DamageType;



	/************  Weapon Effects  ***********/

	//Particle system used as muzzle flash when gun is fired
	UPROPERTY(Category = WeaponEffects, EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* MuzzleEffect;
	//Particle system used when projectile impacts with something
	UPROPERTY(Category = WeaponEffects, EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ImpactEffect;
	//Particle system used as bullet trace when weapon is fired
	UPROPERTY(Category = WeaponEffects, EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* BulletTraceEffect;


	virtual FHitResult GetFiringHitResult(AActor* _gunOwner);
	virtual void PlayFiringEffects(FHitResult _hitInfo);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
