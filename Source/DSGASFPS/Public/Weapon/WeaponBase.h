// copyright DX  GDS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unarmed,
	Rifle,
	Pistol
};

class USkeletalMeshComponent;
class UAnimInstance;
class UAnimMontage;

UCLASS()
class DSGASFPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SkeleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attach")
	FTransform RelativeTransAttach=FTransform(FRotator(0.0, -90.0, 0.0), FVector(-45, 0.0, -25.0), FVector(1.0, 1.0, 1.0));
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attach")
	FName AttachSocketName = TEXT("weapon_r_muzzle");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	TSubclassOf<UAnimInstance> LinkAnimClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	UAnimMontage* SwitchWeaponMontage;

};
