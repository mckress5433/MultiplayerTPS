// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_GunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ATPS_GunBase::ATPS_GunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
}


// Called when the game starts or when spawned
void ATPS_GunBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPS_GunBase::Fire()
{
	AActor* gunOwner = GetOwner();
	if (gunOwner) {

		FVector eyeLocation;
		FRotator eyeRotation;
		gunOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

		FVector traceEnd = eyeLocation + (eyeRotation.Vector() * 10000);
		FVector traceStart = GunMesh->GetSocketLocation(MuzzleSocketName);

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(gunOwner);
		queryParams.AddIgnoredActor(this);
		queryParams.bTraceComplex = true;

		FHitResult hitInfo;
		if (GetWorld()->LineTraceSingleByChannel(hitInfo, traceStart, traceEnd, ECC_Visibility, queryParams)) {
			//if blocking hit then process damage
			AActor* hitActor = hitInfo.GetActor();

			UGameplayStatics::ApplyPointDamage(
				hitActor,
				BaseDamage,
				hitInfo.ImpactNormal,
				hitInfo,
				gunOwner->GetInstigatorController(),
				gunOwner,
				DamageType
			);
		}

		DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::White, false, 1.0f, 0, 1.0f);
	}
}

// Called every frame
void ATPS_GunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

