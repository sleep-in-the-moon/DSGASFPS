// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DGSPlayerState.h"
#include "AbilitySystem/DGSAbilitySystemComponent.h"

ADGSPlayerState::ADGSPlayerState()
{
	NetUpdateFrequency = 100.0f;
	DGSAbilitySystemComponent = CreateDefaultSubobject<UDGSAbilitySystemComponent>(TEXT("ASC"));
	DGSAbilitySystemComponent->SetIsReplicated(true);	//¿ªÆô¸´ÖÆ
}

UAbilitySystemComponent* ADGSPlayerState::GetAbilitySystemComponent() const
{
	return DGSAbilitySystemComponent;
}
