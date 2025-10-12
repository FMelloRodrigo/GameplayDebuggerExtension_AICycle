// Copyright 2025 - Rodrigo Mello

#pragma once

#include "Modules/ModuleManager.h"

class GDebuggerExtension_AICycleModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


};
