// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/Weapon/TPS_GunBase.h"
#include "TPS_Character.generated.h"

//Forward Declarations 
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MULTIPLAYER_TPS_API ATPS_Character : public ACharacter
{
	GENERATED_BODY()

private:
	bool bReadyToCrouch = true;

public:
	// Sets default values for this character's properties
	ATPS_Character();

	UPROPERTY(Category = Weapons, VisibleAnywhere, BlueprintReadWrite)
	ATPS_GunBase* EquipedGun;

protected:
	/********  Character Components  ********/
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/********  Protected Functions  ********/
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Character Control Functions
	void MoveForward(float _value);
	void MoveRight(float _value);

	void CrouchInputPressed();
	void CrouchInputReleased();

	void FireWeapon();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
