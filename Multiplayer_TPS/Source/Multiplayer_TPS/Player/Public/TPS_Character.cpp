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
	
	if (GunClassToSpawn) {

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParams.Owner = this;
		EquipedGun = GetWorld()->SpawnActor<ATPS_GunBase>(GunClassToSpawn, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);

		EquipedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipedGun->GetWeaponSocketName());
	}
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

void ATPS_Character::FireWeapon()
{
	if (EquipedGun) {
		EquipedGun->Fire();
	}
}

// Called every frame
void ATPS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetVelocity().Size() > 0.0f) {
		bUseControllerRotationYaw = true;
	}
	else {
		bUseControllerRotationYaw = false;

		FRotator actorRot = GetActorRotation();
		FRotator controlRot = GetControlRotation();
		FRotator deltaRot = actorRot - controlRot;
		deltaRot.Normalize();

		if (deltaRot.Yaw > 90) {
			float dif = (deltaRot.Yaw - 90)*-1.0f;
			AddActorWorldRotation(FRotator(0.0f, dif, 0.0f));
		}
		else if (deltaRot.Yaw < -90) {
			float dif = (deltaRot.Yaw + 90)*-1.0f;
			AddActorWorldRotation(FRotator(0.0f, dif, 0.0f));
			
		}
	}
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

	PlayerInputComponent->BindAction("FireWeapon", IE_Pressed, this, &ATPS_Character::FireWeapon);

}

FVector ATPS_Character::GetPawnViewLocation() const
{
	if (CameraComponent) {
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

