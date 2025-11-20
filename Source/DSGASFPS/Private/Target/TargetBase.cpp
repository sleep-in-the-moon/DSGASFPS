// copyright DX GDS


#include "Target/TargetBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATargetBase::ATargetBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("asc"));
	ASC->SetIsReplicated(true);
	
}

UAbilitySystemComponent* ATargetBase::GetAbilitySystemComponent() const
{
	return ASC;
}

//void ATargetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ATargetBase, MatIns);
//}

// Called when the game starts or when spawned
void ATargetBase::BeginPlay()
{
	Super::BeginPlay();
	
	/*if (HasAuthority())*/
	{
		if (ASC)
		{
			ASC->InitAbilityActorInfo(this, this);

			ASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetHPAttribute()).AddUObject(this, &ATargetBase::OnHelthChange);
		}
	}

		if (Mesh && Mat)
		{
			MatIns = Mesh->CreateDynamicMaterialInstance(0, Mat);
		}
	
	/*UE_LOG(LogTemp, Warning, TEXT("TargetBeginPlay"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("TargetBeginPlay"));*/
}

void ATargetBase::OnHelthChange(const FOnAttributeChangeData& Data)
{
	if (MatIns && ASC)
	{
		if (Data.NewValue > 0)
		{
			bool temp;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("OnHelthChange"));
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
