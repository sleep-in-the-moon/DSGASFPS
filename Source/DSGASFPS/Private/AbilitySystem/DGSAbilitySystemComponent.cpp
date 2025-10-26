// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DGSAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "Character/DGSBasicCharacter.h"

void UDGSAbilitySystemComponent::BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo)
{
	UEnum* EnumBinds = BindInfo.GetBindEnum();

	UEnhancedInputComponent* EnhancedInputCom = Cast<UEnhancedInputComponent>(InputComponent);
	ADGSBasicCharacter* Character = Cast<ADGSBasicCharacter>(GetAvatarActor());
	if (!EnhancedInputCom || !Character)
		return;

	SetBlockAbilityBindingsArray(BindInfo);

	for (int32 idx = 0; idx < EnumBinds->NumEnums(); ++idx)
	{
		const FString FullStr = EnumBinds->GetNameStringByIndex(idx);

		// Pressed event
		if (Character->InputEnumActionMap.Contains(static_cast<EDGSAbilityInputID>(idx)))
		{
			UInputAction* action =  Character->InputEnumActionMap.FindRef(static_cast<EDGSAbilityInputID>(idx));
			EnhancedInputCom->BindAction(action, ETriggerEvent::Started, this, &UAbilitySystemComponent::AbilityLocalInputPressed, idx);
		}

		// Released event
		if (Character->InputEnumActionMap.Contains(static_cast<EDGSAbilityInputID>(idx)))
		{
			UInputAction* action = Character->InputEnumActionMap.FindRef(static_cast<EDGSAbilityInputID>(idx));
			EnhancedInputCom->BindAction(action, ETriggerEvent::Completed, this, &UAbilitySystemComponent::AbilityLocalInputReleased, idx);
		}
	}

	// Bind Confirm/Cancel. Note: these have to come last!
	if (BindInfo.ConfirmTargetCommand.IsEmpty() == false)
	{
		FInputActionBinding AB(FName(*BindInfo.ConfirmTargetCommand), IE_Pressed);
		AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::LocalInputConfirm);
		InputComponent->AddActionBinding(AB);
	}

	if (BindInfo.CancelTargetCommand.IsEmpty() == false)
	{
		FInputActionBinding AB(FName(*BindInfo.CancelTargetCommand), IE_Pressed);
		AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::LocalInputCancel);
		InputComponent->AddActionBinding(AB);
	}

	if (BindInfo.CancelTargetInputID >= 0)
	{
		GenericCancelInputID = BindInfo.CancelTargetInputID;
	}
	if (BindInfo.ConfirmTargetInputID >= 0)
	{
		GenericConfirmInputID = BindInfo.ConfirmTargetInputID;
	}
}
