// copyright DX GDS

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class DSGASFPS_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HP);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHP);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	void OnRep_HP(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHP(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_HP, Category = "CharacterAttribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHP, Category = "CharacterAttribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHP;

};
