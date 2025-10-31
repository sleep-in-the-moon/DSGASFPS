// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicCharacter.h"
#include "AbilitySystemInterface.h"
#include "Data/DGSType.h"

#include "DGSBasicCharacter.generated.h"

class UInputAction;
class UDGSGameplayAbility;
class AWeaponBase;
struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class DSGASFPS_API ADGSBasicCharacter : public ABasicCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADGSBasicCharacter();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SwitchWeapon(AWeaponBase* NewWeapon);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SpawnNewWeaponAndEquip(TSubclassOf<AWeaponBase> WeaponClass);

private:
	void OnHelthChange(const FOnAttributeChangeData& Data);

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TMap<EDGSAbilityInputID, UInputAction*> InputEnumActionMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UDGSGameplayAbility> > AbilitiesArray;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeaponBase* EquippedWeapon=nullptr;

	UFUNCTION()
	void OnRep_EquippedWeapon();

private:
	bool bIsBindAbilityInput=false;

};
