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

	UPROPERTY(Replicated)
	bool bIsReloading = false;

	UPROPERTY(Replicated)
	bool bWishAimState = false;

	UPROPERTY(Replicated)
	bool bIsSprinting = false;
	float DefaultWalkSpeed;
	UPROPERTY(EditDefaultsOnly)
	float SprintSpeed = 800.0f;

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

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayFiringMontage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayReloadMontage();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRotator GetClientControlRotation();

protected:

	UPROPERTY(Category = "Player State", BlueprintReadOnly, Replicated)
	bool bIsAiming = false;

	UPROPERTY(Category = "Weapons", BlueprintReadOnly, Replicated)
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

	UFUNCTION(Server, Reliable)
	void ServerFireInputPressed();
	UFUNCTION(Server, Reliable)
	void ServerFireInputReleased();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFiringMontage();

	UFUNCTION(Server, Reliable)
	void ServerAimInputPressed();
	UFUNCTION(Server, Reliable)
	void ServerAimInputReleased();
	void UpdateAimState(bool _newAimState);

	UFUNCTION(Server, Reliable)
	void ServerReloadInputPressed();
	UFUNCTION(BlueprintCallable)
	void ReloadFinished(bool _reloadSuccessful);
	UFUNCTION(BlueprintImplementableEvent)
	void PlayReloadMontage();

	UFUNCTION(Server, Reliable)
	void ServerSprintInputPressed();
	UFUNCTION(Server, Reliable)
	void ServerSprintInputReleased();
	void StopSprinting();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
};
