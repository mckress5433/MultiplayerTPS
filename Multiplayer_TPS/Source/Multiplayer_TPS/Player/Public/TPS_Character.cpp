// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "UnrealMathUtility.h"
#include "Public/Components/TPS_HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include <UnrealNetwork.h>


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

	HealthComponent = CreateDefaultSubobject<UTPS_HealthComponent>(TEXT("HealthComponent"));
}


// Called when the game starts or when spawned
void ATPS_Character::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComponent->FieldOfView;
	DefaultCameraLagSpeed = CameraBoom->CameraLagSpeed;
	
	if (GunClassToSpawn) {

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParams.Owner = this;
		EquipedGun = GetWorld()->SpawnActor<ATPS_GunBase>(GunClassToSpawn, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);

		EquipedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipedGun->GetWeaponSocketName());
	}

	HealthComponent->OnHealthChanged.AddDynamic(this, &ATPS_Character::OnHealthChanged);
}

// Called every frame
void ATPS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
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

		ServerSetClientControlRotation(GetControlRotation());
	}
}

FRotator ATPS_Character::GetClientControlRotation()
{
	return ClientControlRotation;
}

void ATPS_Character::ServerSetClientControlRotation_Implementation(FRotator _clientControlRotation)
{
	ClientControlRotation = _clientControlRotation;
}


/*MOVEMENT*/
void ATPS_Character::MoveForward(float _value)
{
	AddMovementInput(GetActorForwardVector()*_value);
}

void ATPS_Character::MoveRight(float _value)
{
	AddMovementInput(GetActorRightVector()*_value);
}


/*CROUCHING*/
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


/*FIRING*/
void ATPS_Character::FireInputPressed()
{
	if (EquipedGun) {
		EquipedGun->BeginFire();
	}
}

void ATPS_Character::FireInputReleased()
{
	if (EquipedGun) {
		EquipedGun->EndFire();
	}
}


/*AIMING*/
void ATPS_Character::AimInputPressed_Implementation()
{
	bWishAimState = true;
	UpdateAimState(bWishAimState);
}

void ATPS_Character::AimInputReleased_Implementation()
{
	bWishAimState = false;
	UpdateAimState(bWishAimState);
}

void ATPS_Character::UpdateAimState(bool _newAimState)
{
	if (_newAimState && !bIsAiming) {
		if (EquipedGun && EquipedGun->GetCanZoom() && !bIsReloading) {
			bIsAiming = true;
			GetWorldTimerManager().SetTimer(ZoomTimerHandle, this, &ATPS_Character::ZoomTimerEvent, 0.01f, true, 0.01f);
			CameraBoom->CameraLagSpeed = EquipedGun->GetZoomedCameraLagSpeed();
		}
	}
	else if(!_newAimState && bIsAiming){
		if (!bIsReloading) {
			bIsAiming = false;
			GetWorldTimerManager().SetTimer(ZoomTimerHandle, this, &ATPS_Character::ZoomTimerEvent, 0.01f, true, 0.01f);
			CameraBoom->CameraLagSpeed = DefaultCameraLagSpeed;
		}
	}
}

void ATPS_Character::ZoomTimerEvent()
{
	float currentFOV = CameraComponent->FieldOfView;
	float targetFOV;

	if (bIsAiming) {
		targetFOV = EquipedGun->GetZoomedFOV();
	}
	else {
		targetFOV = DefaultFOV;
	}

	if (currentFOV != targetFOV) {
		float newFOV = FMath::FInterpTo(currentFOV, targetFOV, 0.01f, EquipedGun->GetZoomInterpSpeed());
		CameraComponent->SetFieldOfView(newFOV);
	}
	else {
		GetWorldTimerManager().ClearTimer(ZoomTimerHandle);
	}
}


/*RELOADING*/
void ATPS_Character::ReloadInputPressed()
{
	if (EquipedGun && EquipedGun->CanReload() && !bIsReloading) {
		bIsReloading = true;
		PlayReloadMontage();
	}
}

void ATPS_Character::ReloadFinished()
{
	bIsReloading = false;
	EquipedGun->Reload();

	UpdateAimState(bWishAimState);
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
	//Gun Controls
	PlayerInputComponent->BindAction("FireWeapon", IE_Pressed, this, &ATPS_Character::FireInputPressed);
	PlayerInputComponent->BindAction("FireWeapon", IE_Released, this, &ATPS_Character::FireInputReleased);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATPS_Character::AimInputPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATPS_Character::AimInputReleased);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ATPS_Character::ReloadInputPressed);
}

FVector ATPS_Character::GetPawnViewLocation() const
{
	if (CameraComponent) {
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void ATPS_Character::OnHealthChanged(UTPS_HealthComponent* _healthComp, float _health, float _healthDelta)
{
	if (_health <= 0.0f && !bDied)
	{
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bDied = true;
	}
}

void ATPS_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPS_Character, ClientControlRotation);
	DOREPLIFETIME(ATPS_Character, bIsAiming);
	DOREPLIFETIME(ATPS_Character, bWishAimState);
}