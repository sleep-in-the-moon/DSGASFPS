// copyright DX  GDS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DGSGameplayAbility.h"
#include "GAShootBase.generated.h"

struct FGameplayAbilityTargetDataHandle;
/**
 * 
 */
UCLASS()
class DSGASFPS_API UGAShootBase : public UDGSGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void StartShoot();
	UFUNCTION(BlueprintImplementableEvent)
	void ShootCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

};
