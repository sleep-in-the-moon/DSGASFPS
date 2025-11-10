// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DGSBasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/DGSAbilitySystemComponent.h"
#include "Character/DGSPlayerState.h"
#include "Data/DGSType.h"
#include "AbilitySystem/DGSGameplayAbility.h"
#include "Weapon/WeaponBase.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/CharacterAttributeSet.h"


ADGSBasicCharacter::ADGSBasicCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void ADGSBasicCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*if (GetLocalRole() == ROLE_Authority)
	{
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, [this]() {
		if (GetAbilitySystemComponent())
		{
			for (TSubclassOf<UDGSGameplayAbility>& ability : AbilitiesArray)
			{
				GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ability, 0, static_cast<int32>(ability.GetDefaultObject()->AbilityInputId), this));
			}
		}
		else
		{

		}
			}, 0.2, false);
		
	}*/
	/*else
	{
		UE_LOG(LogTemp, Warning, TEXT("Role::%d"), GetLocalRole());
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Role::%d"), GetLocalRole()));
	}*/
}

UAbilitySystemComponent* ADGSBasicCharacter::GetAbilitySystemComponent() const
{
	if (ADGSPlayerState* DGSPlayerState = GetPlayerState<ADGSPlayerState>())
		return DGSPlayerState->GetAbilitySystemComponent();

	return nullptr;
}

void ADGSBasicCharacter::PossessedBy(AController* NewController)//服务器
{
	Super::PossessedBy(NewController);

	if (GetAbilitySystemComponent())
	{
		for (TSubclassOf<UDGSGameplayAbility>& ability : AbilitiesArray)
		{
			GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(ability, 0, static_cast<int32>(ability.GetDefaultObject()->AbilityInputId), this));
		}

		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetHPAttribute()).AddUObject(this, &ADGSBasicCharacter::OnHelthChange);
	}

	ADGSPlayerState* DGSPlayerState = GetPlayerState<ADGSPlayerState>();
	if (DGSPlayerState && DGSPlayerState->GetAbilitySystemComponent())
	{
		DGSPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DGSPlayerState, this);
	}

}

void ADGSBasicCharacter::OnRep_PlayerState()//客户端
{
	Super::OnRep_PlayerState();
	ADGSPlayerState* DGSPlayerState = GetPlayerState<ADGSPlayerState>();
	if (DGSPlayerState && DGSPlayerState->GetAbilitySystemComponent())
		DGSPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DGSPlayerState, this);

	if (!bIsBindAbilityInput  && InputComponent)
	{
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/DSGASFPS"), FName("EDGSAbilityInputID"));
		GetAbilitySystemComponent()->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("Confirm"), FString("Cancel"), AbilityEnumAssetPath, static_cast<int32>(EDGSAbilityInputID::Confirm), static_cast<int32>(EDGSAbilityInputID::Cancel)));
		bIsBindAbilityInput = true;
	}
}

void ADGSBasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!bIsBindAbilityInput && GetAbilitySystemComponent() && IsLocallyControlled())
	{
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/DSGASFPS"), FName("EDGSAbilityInputID"));
		GetAbilitySystemComponent()->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("Confirm"), FString("Cancel"), AbilityEnumAssetPath, static_cast<int32>(EDGSAbilityInputID::Confirm), static_cast<int32>(EDGSAbilityInputID::Cancel)));
		bIsBindAbilityInput = true;
	}
}

void ADGSBasicCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADGSBasicCharacter, EquippedWeapon);
}

void ADGSBasicCharacter::OnHelthChange(const FOnAttributeChangeData& Data)
{
	DG_HelthChange.Broadcast(Data.NewValue);
}

void ADGSBasicCharacter::SwitchWeapon_Implementation(AWeaponBase* NewWeapon)
{
	if (/*NewWeapon &&*/ EquippedWeapon != NewWeapon)
		EquippedWeapon = NewWeapon;
}

void ADGSBasicCharacter::SpawnNewWeaponAndEquip_Implementation(TSubclassOf<AWeaponBase> WeaponClass)
{
	if (WeaponClass)
	{
		if (EquippedWeapon)
			EquippedWeapon->Destroy();

		SwitchWeapon(GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, FTransform::Identity));
	}
}

void ADGSBasicCharacter::OnRep_EquippedWeapon()
{
	if (EquippedWeapon)
	{
		if (EquippedWeapon->LinkAnimClass && GetMesh())
		{
			GetMesh()->LinkAnimClassLayers(EquippedWeapon->LinkAnimClass);
		}
		if (EquippedWeapon->SwitchWeaponMontage)
		{
			PlayAnimMontage(EquippedWeapon->SwitchWeaponMontage);
		}

		EquippedWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, false)
			, EquippedWeapon->AttachSocketName);

		EquippedWeapon->SetActorRelativeTransform(EquippedWeapon->RelativeTransAttach);
	}
}
