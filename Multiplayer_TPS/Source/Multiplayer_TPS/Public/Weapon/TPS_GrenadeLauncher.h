// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TPS_GunBase.h"
#include "TPS_Grenade.h"
#include "TPS_GrenadeLauncher.generated.h"


UCLASS()
class MULTIPLAYER_TPS_API ATPS_GrenadeLauncher : public ATPS_GunBase
{
	GENERATED_BODY()

public:

	virtual void Fire() override;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATPS_Grenade> ProjectileClass;
};
