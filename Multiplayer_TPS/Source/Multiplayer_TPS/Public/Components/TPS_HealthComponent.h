// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPS_HealthComponent.generated.h"


UCLASS( ClassGroup=(TPS), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYER_TPS_API UTPS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPS_HealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Category = "HealthComponent", BlueprintReadOnly, EditDefaultsOnly)
	float Health;
	UPROPERTY(Category = "HealthComponent", BlueprintReadOnly, EditDefaultsOnly)
	float MaxHealth;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//Adds HealthDelta to current health
	void UpdateHealth(float _HealthDelta);
		
};
