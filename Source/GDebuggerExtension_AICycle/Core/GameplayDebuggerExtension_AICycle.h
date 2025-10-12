// Copyright 2025 - Rodrigo Mello
#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerExtension.h"

#include "GDebuggerExtension_AICycle/Settings/AICycleDebuggerSettings.h"

class APawn;

class GameplayDebuggerExtension_AICycle : public FGameplayDebuggerExtension
{
public:
	
	GameplayDebuggerExtension_AICycle();

	virtual void OnActivated() override;
	virtual void OnDeactivated() override;
	virtual FString GetDescription() const override;


	static TSharedRef<FGameplayDebuggerExtension> MakeInstance();

protected:

	void OnKey_PageUp();
	void OnKey_PageDown();
	void OnRefreshTargets();
	void GatherTargets();

	void SetDebuggerTarget(APawn* Pawn);

protected:
	TArray<TWeakObjectPtr<APawn>> CachedAITargets;
	int32 CurrentIndex;

	int32 PageDownBindingIdx;
	int32 PageUpBindingIdx;
	int32 ResetBindingIdx;

};