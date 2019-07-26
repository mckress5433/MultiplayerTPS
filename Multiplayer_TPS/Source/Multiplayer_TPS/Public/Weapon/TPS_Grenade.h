// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS_Grenade.generated.h"

class USkeletalMeshComponent;
class UProjectileMovementComponent;
class URadialForceComponent;

UCLASS()
class MULTIPLAYER_TPS_API ATPS_Grenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPS_Grenade();

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GrenadeMesh;
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComp;

	UFUNCTION(BlueprintCallable)
	virtual void PrimeGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ExplosionEffect;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URadialForceComponent* RadialForceComp;

	void Detonate();
	void DestroyGrenade();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
