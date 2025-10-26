// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DGSAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class DSGASFPS_API UDGSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	virtual void BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo) override;

};
