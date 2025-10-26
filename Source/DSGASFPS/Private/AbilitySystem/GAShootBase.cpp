// copyright DX  GDS


#include "AbilitySystem/GAShootBase.h"
#include "AIController.h"

void UGAShootBase::StartShoot()
{
	APawn* pawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	AController* Controller = pawn->GetController();
	if (!pawn || !Controller)
		return;

	FVector StartLoc;
	FRotator Rot;
	if (APlayerController* playerControl = Cast<APlayerController>(Controller)) //Íæ¼Ò
	{
		playerControl->GetPlayerViewPoint(StartLoc, Rot);
	}
	else if (AAIController* AIControl = Cast<AAIController>(Controller)) //AI
	{
		Rot = Controller->GetControlRotation();
		StartLoc = pawn->GetActorLocation() + FVector(0, 0, pawn->BaseEyeHeight);
	}

	FTransform StartTrans(Rot, StartLoc);
	TArray<FHitResult> HitResults;
	GetWorld()->LineTraceMultiByChannel(HitResults, StartLoc, StartLoc+ StartTrans.GetUnitAxis(EAxis::X)*7000, ECollisionChannel::ECC_Pawn);
	DrawDebugLine(GetWorld(), StartLoc, StartLoc + StartTrans.GetUnitAxis(EAxis::X) * 7000, FColor::Green, false, 3.0);

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	for (const FHitResult& HitRes : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit;
		TargetData->HitResult = HitRes;
		TargetDataHandle.Add(TargetData);
	}

	ShootCallback(TargetDataHandle);

	CurrentActorInfo;
}
