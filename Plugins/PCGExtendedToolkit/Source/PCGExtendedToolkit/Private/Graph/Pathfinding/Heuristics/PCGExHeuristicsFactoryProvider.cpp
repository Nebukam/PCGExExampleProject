﻿// Copyright 2024 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/

#include "Graph/Pathfinding/Heuristics/PCGExHeuristicsFactoryProvider.h"

#define LOCTEXT_NAMESPACE "PCGExCreateHeuristics"
#define PCGEX_NAMESPACE CreateHeuristics

void FPCGExHeuristicConfigBase::Init()
{
	if (!bUseLocalCurve) { LocalScoreCurve.ExternalCurve = ScoreCurve.Get(); }
	ScoreCurveObj = LocalScoreCurve.GetRichCurveConst();
}

UPCGExHeuristicOperation* UPCGExHeuristicsFactoryBase::CreateOperation(FPCGExContext* InContext) const
{
	return nullptr; // Create heuristic operation
}

UPCGExParamFactoryBase* UPCGExHeuristicsFactoryProviderSettings::CreateFactory(FPCGExContext* InContext, UPCGExParamFactoryBase* InFactory) const
{
	return InFactory;
}

#undef LOCTEXT_NAMESPACE
#undef PCGEX_NAMESPACE
