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
class UTPS_HealthComponent;

UCLASS()
class MULTIPLAYER_TPS_API ATPS_Character : public ACharacter
{
	GENERATED_BODY()

private:

	bool bReadyToCrouch = true;
	bool bIsReloading = false;

	UPROPERTY(Replicated)
	bool bWishAimState = false;

	bool bDied = false;

	//Default Field Of View for player when not aiming down sights
	float DefaultFOV;
	float DefaultCameraLagSpeed;

	UPROPERTY(Replicated)
	FRotator ClientControlRotation;
	UFUNCTION(Server, Reliable)
	void ServerSetClientControlRotation(FRotator _clientControlRotation);

	FTimerHandle ZoomTimerHandle;

	void ZoomTimerEvent();

public:
	// Sets default values for this character's properties
	ATPS_Character();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFiringMontage();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRotator GetClientControlRotation();

protected:

	UPROPERTY(Category = "Player State", BlueprintReadOnly, Replicated)
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

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTPS_HealthComponent* HealthComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnHealthChanged(UTPS_HealthComponent* _healthComp, float _health, float _healthDelta);

	/*Character Control Functions*/
	void MoveForward(float _value);
	void MoveRight(float _value);

	void CrouchInputPressed();
	void CrouchInputReleased();

	void FireInputPressed();
	void FireInputReleased();

	UFUNCTION(Server, Reliable)
	void AimInputPressed();
	UFUNCTION(Server, Reliable)
	void AimInputReleased();
	void UpdateAimState(bool _newAimState);

	void ReloadInputPressed();
	UFUNCTION(BlueprintCallable)
	void ReloadFinished();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayReloadMontage();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
};
