// copyright DX GDS


#include "Target/TargetBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATargetBase::ATargetBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("asc"));
}

UAbilitySystemComponent* ATargetBase::GetAbilitySystemComponent() const
{
	return ASC;
}

// Called when the game starts or when spawned
void ATargetBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetHPAttribute()).AddUObject(this, &ATargetBase::OnHelthChange);
	}

	if (Mesh && Mat)
	{
		MatIns = Mesh->CreateDynamicMaterialInstance(0, Mat);
	}
}

void ATargetBase::OnHelthChange(const FOnAttributeChangeData& Data)
{
	if (MatIns && ASC)
	{
		if (Data.NewValue > 0)
		{
			bool temp;
			MatIns->SetScalarParameterValue(TEXT("Helth"), Data.NewValue / ASC->GetGameplayAttributeValue(UCharacterAttributeSet::GetMaxHPAttribute(), temp));
		}
		else
		{
			if(DestroySound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySound, GetActorLocation());

			/*SetActorHiddenInGame(true);
			SetActorTickEnabled(false);*/
			Destroy();
		}
		
	}
}

// Called every frame
void ATargetBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
