// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS_GunBase.generated.h"

class USkeletalMeshComponent;

UCLASS()
class MULTIPLAYER_TPS_API ATPS_GunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPS_GunBase();

	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunMesh;

	//The character socket that the gun attaches to
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponSocketName = "RifleSocket";
	//The socket that the left hand of the character will attach to
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	FName SecondHandSocketName = "SecondHandSocket";
	//The socket that represents the muzzle of the gun
	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage = 10.0f;

	UPROPERTY(Category = WeaponProperties, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDamageType> DamageType;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
