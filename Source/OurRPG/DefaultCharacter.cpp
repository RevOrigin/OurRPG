// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
ADefaultCharacter::ADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* RootCapsule = GetCapsuleComponent();

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootCapsule);
	SpringArmComponent->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	SpringArmComponent->TargetArmLength = 200.f;

	CharacterCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCameraComponent"));
	check(CharacterCameraComponent != nullptr);

	CharacterCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 根据缩放比率设置foc以及弹簧臂长度
	CharacterCameraComponent->FieldOfView = FMath::Lerp<float>(90, 60, ZoomFactor);
	SpringArmComponent->TargetArmLength = FMath::Lerp<float>(400, 300, ZoomFactor);

	// 更新角色的旋转（只考虑yaw)
	FRotator PlayerRotator = GetActorRotation();
	PlayerRotator.Yaw += CameraInput.X;
	SetActorRotation(PlayerRotator);

	// 更新相机的pitch
	FRotator CameraRotator = SpringArmComponent->GetComponentRotation();
	CameraRotator.Pitch = FMath::Clamp<float>(CameraRotator.Pitch + CameraInput.Y, -80.0f, 60.0f);
	SpringArmComponent->SetWorldRotation(CameraRotator);
}

// Called to bind functionality to input
void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ADefaultCharacter::YawCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &ADefaultCharacter::PitchCamera);
	PlayerInputComponent->BindAction("ZoomIn", IE_Released, this, &ADefaultCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Released, this, &ADefaultCharacter::ZoomOut);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ADefaultCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ADefaultCharacter::StopJump);

}

void ADefaultCharacter::MoveForward(float Value) {
	FVector Direction = GetActorForwardVector();
	AddMovementInput(Direction, Value);
}

void ADefaultCharacter::MoveRight(float Value)
{
	FVector Direction = GetActorRightVector();
	AddMovementInput(Direction, Value);
}

void ADefaultCharacter::StartJump()
{
	bPressedJump = true;
}

void ADefaultCharacter::StopJump()
{
	bPressedJump = false;
}

void ADefaultCharacter::PitchCamera(float value) {
	CameraInput.Y = value;
}

void ADefaultCharacter::YawCamera(float value) {
	CameraInput.X = value;
}

void ADefaultCharacter::ZoomIn() {
	ZoomFactor += 0.25f;
	ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0, 1);
}

void ADefaultCharacter::ZoomOut() {
	ZoomFactor -= 0.25f;
	ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0, 1);
}


