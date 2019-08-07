// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_HealthComponent.h"
#include <GameFramework/Actor.h>
#include <UnrealMathUtility.h>


static int32 DebugHealth = 0;
static FAutoConsoleVariableRef CVARDebugHealth(
	TEXT("TPS.DebugHealth"),
	DebugHealth,
	TEXT("Prints Health values when updated"),
	ECVF_Cheat
);

// Sets default values for this component's properties
UTPS_HealthComponent::UTPS_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Health = 100.0f;
	MaxHealth = 100.0f;
}


// Called when the game starts
void UTPS_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	AActor* myOwner = GetOwner();
	if (myOwner) 
	{
		myOwner->OnTakeAnyDamage.AddDynamic(this, &UTPS_HealthComponent::HandleTakeAnyDamage);
	}
}

void UTPS_HealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0) {
		return;
	}
	UpdateHealth(Damage*-1.0f);
}

void UTPS_HealthComponent::UpdateHealth(float _HealthDelta)
{
	//Update Health Clamped
	Health = FMath::Clamp(Health + _HealthDelta, 0.0f, MaxHealth);

	if (DebugHealth > 0) {
		UE_LOG(LogTemp, Log, TEXT("Health changed: %s"), *FString::SanitizeFloat(Health));
	}
}

