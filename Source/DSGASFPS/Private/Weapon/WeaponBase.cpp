// copyright DX  GDS


#include "Weapon/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	RootComponent = SkeleMesh;
	//SkeleMesh->SetupAttachment(RootComponent);

	SkeleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
