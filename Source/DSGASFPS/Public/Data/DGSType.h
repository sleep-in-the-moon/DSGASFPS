#pragma once
//#include "CoreMinimal.h"

#include "DGSType.generated.h"

UENUM(BlueprintType)
enum class EDGSAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Shoot,
	Jump,
	Reload
};
