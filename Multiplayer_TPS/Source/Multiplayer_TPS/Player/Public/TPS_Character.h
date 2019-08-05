// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/Weapon/TPS_GunBase.h"
#include "Engine/EngineTypes.h"
#include "TPS_Character.generated.h"

//Forward Declarations 
class UCameraComponent;
class USpringArmComponent;
class ATPS_GunBase;

UCLASS()
class MULTIPLAYER_TPS_API ATPS_Character : public ACharacter
{
	GENERATED_BODY()

private:

	bool bReadyToCrouch = true;

	//Default Field Of View for player when not aiming down sights
	float DefaultFOV;
	float DefaultCameraLagSpeed;

	FTimerHandle ZoomTimerHandle;

	void ZoomTimerEvent();

public:
	// Sets default values for this character's properties
	ATPS_Character();

protected:

	UPROPERTY(Category = "Player State", BlueprintReadOnly)
	bool bIsAiming = false;

	UPROPERTY(Category = "Weapons", BlueprintReadOnly)
	ATPS_GunBase* EquipedGun;

	/********  Character Components  ********/
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(Category = "Weapons", EditDefaultsOnly)
	TSubclassOf<ATPS_GunBase> GunClassToSpawn;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/*Character Control Functions*/
	void MoveForward(float _value);
	void MoveRight(float _value);

	void CrouchInputPressed();
	void CrouchInputReleased();

	void FireWeapon();

	void AimInputPressed();
	void AimInputReleased();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
};
