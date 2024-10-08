﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#include "Graph/Pathfinding/Heuristics/PCGExHeuristicFeedback.h"

void UPCGExHeuristicFeedback::PrepareForCluster(const PCGExCluster::FCluster* InCluster)
{
	MaxNodeWeight = 0;
	MaxEdgeWeight = 0;
	Super::PrepareForCluster(InCluster);
}

void UPCGExHeuristicFeedback::Cleanup()
{
	NodeExtraWeight.Empty();
	EdgeExtraWeight.Empty();
	Super::Cleanup();
}

UPCGExHeuristicOperation* UPCGExHeuristicsFactoryFeedback::CreateOperation(FPCGExContext* InContext) const
{
	UPCGExHeuristicFeedback* NewOperation = InContext->ManagedObjects->New<UPCGExHeuristicFeedback>();
	PCGEX_FORWARD_HEURISTIC_CONFIG
	NewOperation->NodeScale = Config.VisitedPointsWeightFactor;
	NewOperation->EdgeScale = Config.VisitedEdgesWeightFactor;
	return NewOperation;
}

UPCGExParamFactoryBase* UPCGExHeuristicFeedbackProviderSettings::CreateFactory(FPCGExContext* InContext, UPCGExParamFactoryBase* InFactory) const
{
	UPCGExHeuristicsFactoryFeedback* NewFactory = InContext->ManagedObjects->New<UPCGExHeuristicsFactoryFeedback>();
	PCGEX_FORWARD_HEURISTIC_FACTORY
	return Super::CreateFactory(InContext, NewFactory);
}

#if WITH_EDITOR
FString UPCGExHeuristicFeedbackProviderSettings::GetDisplayName() const
{
	return GetDefaultNodeName().ToString()
		+ TEXT(" @ ")
		+ FString::Printf(TEXT("%.3f"), (static_cast<int32>(1000 * Config.WeightFactor) / 1000.0));
}
#endif
