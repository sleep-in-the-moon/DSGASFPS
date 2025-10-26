// copyright DX  GDS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Data/DGSType.h"
#include "DGSGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DSGASFPS_API UDGSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	EDGSAbilityInputID AbilityInputId = EDGSAbilityInputID::None;
	UPROPERTY(EditDefaultsOnly)
	EDGSAbilityInputID AbilityId = EDGSAbilityInputID::None;

};
