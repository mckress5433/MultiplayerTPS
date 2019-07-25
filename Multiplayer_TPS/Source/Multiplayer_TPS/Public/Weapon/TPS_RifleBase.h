// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TPS_PistolBase.h"
#include "TPS_RifleBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_TPS_API ATPS_RifleBase : public ATPS_PistolBase
{
	GENERATED_BODY()
	
public:

	virtual void Fire() override;

protected:
};
