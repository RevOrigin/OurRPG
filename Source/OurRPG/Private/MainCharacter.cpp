// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "MainAttributeComponent.h"
#include "MainCharacterAnim.h"
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	// 由pawn控制旋转
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	// 相机绑到SpringArm上
	CameraComponent->SetupAttachment(SpringArmComponent);

	// todo:interaction component, action

	AttributeComponent = CreateDefaultSubobject<UMainAttributeComponent>("AttributeComponent");
	
	// 移动时将人物旋转到加速方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// 启用碰撞检测
	GetMesh()->SetGenerateOverlapEvents(true);
	// 胶囊不产生碰撞
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	TimeToHitParamName = "TimeToHit";
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 绑定事件
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AMainCharacter::OnHealthChanged);
}

// 将视角位置设置为相机位置
FVector AMainCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}



// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::PrimiryInteract);

	PlayerInputComponent->BindAction("Fly", IE_Pressed, this, &AMainCharacter::FlyStart);
	PlayerInputComponent->BindAction("Fly", IE_Released, this, &AMainCharacter::FlyStop);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

void AMainCharacter::HealSelf(float Amount)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

void AMainCharacter::MoveForward(float Value)
{
	// 获取当前的旋转值
	FRotator Rot = GetControlRotation();
	Rot.Pitch = 0.f;
	Rot.Roll = 0.f;
	UE_LOG(LogTemp, Log, TEXT("direction:%s"), *Rot.ToString());
	AddMovementInput(Rot.Vector(), Value);
}

void AMainCharacter::MoveRight(float Value)
{
	FRotator Rot = GetControlRotation();
	Rot.Pitch = 0.f;
	Rot.Roll = 0.f;

	FVector RightVector = FRotationMatrix(Rot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AMainCharacter::FlyStart()
{
	// todo:转发给action
}

void AMainCharacter::FlyStop()
{
	// todo
}

void AMainCharacter::Attack()
{
	// 超过10帧，断开连击
	if (GFrameNumber - LastAttackFrame > 20)
	{
		CurrentAttackChain = 0;
	}
	LastAttackFrame = GFrameNumber;

	if (bIsAttacking)
	{
		return;
	}

	PlayAttackAnim(CurrentAttackChain);
	// todo:action
	TakingBlade = true;
}

void AMainCharacter::AttackEnd()
{
	UE_LOG(LogTemp,Log, TEXT("AttackEnd"));
	CurrentAttackChain = (CurrentAttackChain + 1) % 4;
	bIsAttacking = false;
	LastAttackFrame = GFrameNumber;
	
}


void AMainCharacter::PlayAttackAnim(int chain)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!bIsAttacking && AnimMontage && AnimInstance && !AnimInstance->Montage_IsPlaying(AnimMontage))
	{
		AnimInstance->Montage_Play(AnimMontage);
		FString name = "Attack" + FString::FromInt(chain);
		AnimInstance->Montage_JumpToSection(FName(*name), AnimMontage);
	}
}


void AMainCharacter::Dash()
{
	// todo:action
}

void AMainCharacter::PrimiryInteract()
{
	// todo:interact
}

void AMainCharacter::OnHealthChanged(AActor* InstigatorActor, UMainAttributeComponent* OwningCom, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		// 受到伤害
		// todo:动画
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		float RageDelta = FMath::Abs(Delta);
		AttributeComponent->ApplyRage(InstigatorActor, RageDelta);
	}

	if (NewHealth <= .0f && Delta < .0f) {}
	{
		// 死亡

		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}

void AMainCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FVector LineStart = GetActorLocation();

	LineStart += GetActorRightVector() * 100.f;

	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.f);

	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, 1.0f, FColor::Yellow, false, .0f, 0, 1.0f);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, 1.0f, FColor::Green, false, .0f, 0, 1.0f);

}


void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	// 创建默认的武器
	// SnapToTargetNotIncludingScale:保持物体缩放
	CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClass);

	if (CurrentWeapon)
	{
		
		// auto rotator = CurrentWeapon->GetWeaponMesh()->GetRelativeRotation().Euler();
		// rotator.Z = -90.f;
		// CurrentWeapon->GetWeaponMesh()->SetRelativeRotation(FRotator::MakeFromEuler(rotator));
		//CurrentWeapon->GetWeaponMesh()->SetupAttachment(GetMesh(), TEXT("right_weapon"));

		CurrentWeapon->GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("right_weapon"));
	}
	
}










