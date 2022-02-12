// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	// 加载网格资源
    if (!WeaponMeshAsset)
    {
	    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Weapons/Mesh/katana"));
	    if (MeshAsset.Succeeded())
	    {
		    WeaponMeshAsset = MeshAsset.Object;
	    }
    }

    if (WeaponMeshAsset)
    {
	    WeaponMesh->SetStaticMesh(WeaponMeshAsset);
    	UE_LOG(LogTemp, Log, TEXT("set weapon mesh"));
    }
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* AWeaponActor::GetWeaponMesh()
{
	return WeaponMesh;
}


