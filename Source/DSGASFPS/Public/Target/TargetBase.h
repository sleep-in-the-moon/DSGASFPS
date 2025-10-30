// copyright DX GDS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"

#include "TargetBase.generated.h"

class UAbilitySystemComponent;
struct FOnAttributeChangeData;

UCLASS()
class DSGASFPS_API ATargetBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnHelthChange(const FOnAttributeChangeData& Data);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface* Mat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* DestroySound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* ASC;

private:
	UMaterialInstanceDynamic* MatIns;

};
