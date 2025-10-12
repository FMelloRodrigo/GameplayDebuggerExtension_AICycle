// Copyright 2025 - Rodrigo Mello

#include "GameplayDebuggerExtension_AICycle.h"

#include "GameplayDebugger.h"
#include "GameplayDebuggerAddonManager.h"
#include "GameplayDebuggerTypes.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "EngineUtils.h"
#include "AIController.h"
#include "GameplayDebuggerCategoryReplicator.h"
#include "GameFramework/PlayerController.h"


GameplayDebuggerExtension_AICycle::GameplayDebuggerExtension_AICycle()
{
	CurrentIndex = 0;

	const FGameplayDebuggerInputHandlerConfig PageDownKeyConfig(TEXT("CycleUp"), EKeys::PageDown.GetFName(), FGameplayDebuggerInputModifier::Ctrl);
	const FGameplayDebuggerInputHandlerConfig PageUpKeyConfig(TEXT("CycleDown"), EKeys::PageUp.GetFName(), FGameplayDebuggerInputModifier::Ctrl);
	const FGameplayDebuggerInputHandlerConfig ResetKeyConfig(TEXT("ResetCycle"), EKeys::R.GetFName(), FGameplayDebuggerInputModifier::Ctrl);

	const bool bHasCycleUpBinding = BindKeyPress(PageDownKeyConfig, this, &GameplayDebuggerExtension_AICycle::OnKey_PageDown);
	PageDownBindingIdx = bHasCycleUpBinding ? (GetNumInputHandlers() - 1) : INDEX_NONE;

	const bool bHasCycleDownBinding = BindKeyPress(PageUpKeyConfig, this, &GameplayDebuggerExtension_AICycle::OnKey_PageUp);
	PageUpBindingIdx = bHasCycleDownBinding ? (GetNumInputHandlers() - 1) : INDEX_NONE;

	const bool bHasResetCycleBinding = BindKeyPress(ResetKeyConfig, this, &GameplayDebuggerExtension_AICycle::OnRefreshTargets);
	ResetBindingIdx = bHasResetCycleBinding ? (GetNumInputHandlers() - 1) : INDEX_NONE;

}

TSharedRef<FGameplayDebuggerExtension> GameplayDebuggerExtension_AICycle::MakeInstance()
{
	return MakeShareable(new GameplayDebuggerExtension_AICycle());
}

void GameplayDebuggerExtension_AICycle::OnActivated()
{
	GatherTargets();
}

void GameplayDebuggerExtension_AICycle::OnDeactivated()
{
	CachedAITargets.Empty();
	CurrentIndex = INDEX_NONE;
}

FString GameplayDebuggerExtension_AICycle::GetDescription() const
{
	return FString::Printf(TEXT("{white} %s or %s :{grey} Cycle AI agents {white} %s:{grey} Reset Cycling"),
		*GetInputHandlerDescription(PageDownBindingIdx),
		*GetInputHandlerDescription(PageUpBindingIdx),
		*GetInputHandlerDescription(ResetBindingIdx));
}

void GameplayDebuggerExtension_AICycle::OnKey_PageUp()
{
	if (CachedAITargets.Num() == 0)
	{
		GatherTargets();
		if (CachedAITargets.Num() == 0) return;
	}

	CurrentIndex = (CurrentIndex + 1) % CachedAITargets.Num();

	if (CachedAITargets.IsValidIndex(CurrentIndex))
	{
		if (APawn* Pawn = CachedAITargets[CurrentIndex].Get())
		{
			SetDebuggerTarget(Pawn);
		}
	}
}

void GameplayDebuggerExtension_AICycle::OnKey_PageDown()
{
	if (CachedAITargets.Num() == 0)
	{
		GatherTargets();
		if (CachedAITargets.Num() == 0) return;
	}

	CurrentIndex = (CurrentIndex - 1 + CachedAITargets.Num()) % CachedAITargets.Num();

	if (CachedAITargets.IsValidIndex(CurrentIndex))
	{
		if (APawn* Pawn = CachedAITargets[CurrentIndex].Get())
		{
			SetDebuggerTarget(Pawn);
		}
	}
}

void GameplayDebuggerExtension_AICycle::OnRefreshTargets()
{
	CurrentIndex = 0;
	GatherTargets();
}

void GameplayDebuggerExtension_AICycle::SetDebuggerTarget(APawn* Pawn)
{
	if (!Pawn) return;
	if (APlayerController* PC = GetPlayerController())
	{
#if WITH_GAMEPLAY_DEBUGGER
		if (AGameplayDebuggerCategoryReplicator* Replicator = GetReplicator())
		{
			Replicator->SetDebugActor(Pawn, false);
		}
#endif
	}
}

void GameplayDebuggerExtension_AICycle::GatherTargets()
{
	CachedAITargets.Reset();

	APlayerController* PC = GetPlayerController();
	UWorld* World = PC ? PC->GetWorld() : nullptr;
	if (!World)
	{
		return;
	}

	AActor* PlayerPawn = PC->GetPawn();
	if (!PlayerPawn)
	{
		return;
	}

	const UAICycleDebuggerSettings* Settings = GetDefault<UAICycleDebuggerSettings>();
	const FVector PlayerLocation = PlayerPawn->GetActorLocation();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(AICycleDebugger), false);
	CollisionParams.AddIgnoredActor(PlayerPawn);

	World->OverlapMultiByObjectType(OverlapResults, PlayerLocation, FQuat::Identity, FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), FCollisionShape::MakeSphere(Settings->SearchRadius), CollisionParams);

	for (const FOverlapResult& Result : OverlapResults)
	{
		if (APawn* Pawn = Cast<APawn>(Result.GetActor()))
		{
			if (Pawn->GetController() && Pawn->GetController()->IsA<AAIController>())
			{
				if ((Settings->AIFilterClasses.Num() == 0))
				{
					CachedAITargets.AddUnique(Pawn);
				}
				else if (Settings->AIFilterClasses.Contains(Pawn->GetClass()))
				{
					CachedAITargets.AddUnique(Pawn);
				}
			}
		}
	}

	if (CachedAITargets.Num() > 1)
	{

		Algo::Sort(CachedAITargets, [&PlayerLocation](const TWeakObjectPtr<AActor>& A, const TWeakObjectPtr<AActor>& B)
			{
				if (!A.IsValid() || !B.IsValid())
				{
					return !A.IsValid();
				}
				const double DistA = FVector::DistSquared(A->GetActorLocation(), PlayerLocation);
				const double DistB = FVector::DistSquared(B->GetActorLocation(), PlayerLocation);
				return DistA < DistB;
			});
	}

	if (CachedAITargets.Num() == 0)
	{
		CurrentIndex = 0;
	}
	else
	{
		CurrentIndex = FMath::Clamp(CurrentIndex, 0, CachedAITargets.Num() - 1);
	}

}