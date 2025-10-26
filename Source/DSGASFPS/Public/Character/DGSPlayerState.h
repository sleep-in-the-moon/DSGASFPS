// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "DGSPlayerState.generated.h"


class UDGSAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DSGASFPS_API ADGSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADGSPlayerState();

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UDGSAbilitySystemComponent* DGSAbilitySystemComponent;
	
};
