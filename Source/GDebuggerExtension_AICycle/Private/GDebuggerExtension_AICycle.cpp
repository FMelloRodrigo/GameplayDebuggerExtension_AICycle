// Copyright 2025 - Rodrigo Mello

#include "GDebuggerExtension_AICycle.h"

#include "Modules/ModuleManager.h"
#include "GameplayDebuggerAddonManager.h"
#include "GDebuggerExtension_AICycle/Core/GameplayDebuggerExtension_AICycle.h" 


IMPLEMENT_MODULE(GDebuggerExtension_AICycleModule, GDebuggerExtension_AICycle);


void GDebuggerExtension_AICycleModule::StartupModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& Debugger = IGameplayDebugger::Get();
	Debugger.RegisterExtension(TEXT("AICycleDebugger"), IGameplayDebugger::FOnGetExtension::CreateStatic(&GameplayDebuggerExtension_AICycle::MakeInstance));
#endif
}

void GDebuggerExtension_AICycleModule::ShutdownModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger::Get().UnregisterExtension(TEXT("AICycleDebugger"));
	}
#endif
}

	




