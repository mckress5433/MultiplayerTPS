// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATPS_Character::ATPS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Called when the game starts or when spawned
void ATPS_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPS_Character::MoveForward(float _value)
{
	AddMovementInput(GetActorForwardVector()*_value);
}

void ATPS_Character::MoveRight(float _value)
{
	AddMovementInput(GetActorRightVector()*_value);
}


void ATPS_Character::CrouchInputPressed()
{
	if (bReadyToCrouch) {
		bReadyToCrouch = false;
		if (GetCharacterMovement()->IsCrouching()) {
			UnCrouch();
		}
		else {
			Crouch();
		}
	}
}

void ATPS_Character::CrouchInputReleased()
{
	bReadyToCrouch = true;
}

// Called every frame
void ATPS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement Input
	PlayerInputComponent->BindAxis("Move_X", this, &ATPS_Character::MoveForward);
	PlayerInputComponent->BindAxis("Move_Y", this, &ATPS_Character::MoveRight);
	//Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPS_Character::Jump);
	
	//Camera Rotation
	PlayerInputComponent->BindAxis("LookUp", this, &ATPS_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ATPS_Character::AddControllerYawInput);
	//Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPS_Character::CrouchInputPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPS_Character::CrouchInputReleased);
}

