﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"

#include "PCGExGraph.h"
#include "PCGExIntersections.h"
#include "PCGExPointsProcessor.h"
#include "PCGExDetailsIntersection.h"
#include "Data/PCGExData.h"
#include "Data/Blending/PCGExUnionBlender.h"
#include "Data/Blending/PCGExMetadataBlender.h"

namespace PCGExGraph
{
	struct /*PCGEXTENDEDTOOLKIT_API*/ FUnionProcessor : TSharedFromThis<FUnionProcessor>
	{
		FPCGExPointsProcessorContext* Context = nullptr;

		TSharedRef<PCGExData::FFacade> UnionDataFacade;
		TSharedPtr<FUnionGraph> UnionGraph;

		FPCGExPointPointIntersectionDetails PointPointIntersectionDetails;

		bool bDoPointEdge = false;
		FPCGExPointEdgeIntersectionDetails PointEdgeIntersectionDetails;
		bool bUseCustomPointEdgeBlending = false;
		FPCGExBlendingDetails CustomPointEdgeBlendingDetails;

		bool bDoEdgeEdge = false;
		FPCGExEdgeEdgeIntersectionDetails EdgeEdgeIntersectionDetails;
		bool bUseCustomEdgeEdgeBlending = false;
		FPCGExBlendingDetails CustomEdgeEdgeBlendingDetails;

		FPCGExGraphBuilderDetails GraphBuilderDetails;


		TSharedPtr<PCGExDataBlending::FUnionBlender> UnionPointsBlender;

		explicit FUnionProcessor(
			FPCGExPointsProcessorContext* InContext,
			TSharedRef<PCGExData::FFacade> InUnionDataFacade,
			TSharedRef<FUnionGraph> InUnionGraph,
			FPCGExPointPointIntersectionDetails PointPointIntersectionDetails,
			FPCGExBlendingDetails InDefaultPointsBlending,
			FPCGExBlendingDetails InDefaultEdgesBlending);

		~FUnionProcessor();

		void InitPointEdge(
			const FPCGExPointEdgeIntersectionDetails& InDetails,
			const bool bUseCustom = false,
			const FPCGExBlendingDetails* InOverride = nullptr);

		void InitEdgeEdge(
			const FPCGExEdgeEdgeIntersectionDetails& InDetails,
			const bool bUseCustom = false,
			const FPCGExBlendingDetails* InOverride = nullptr);

		bool StartExecution(
			const TArray<TSharedRef<PCGExData::FFacade>>& InFacades,
			const FPCGExGraphBuilderDetails& InBuilderDetails,
			const FPCGExCarryOverDetails* InCarryOverDetails);

		bool Execute();

	protected:
		bool bRunning = false;

		int32 NewEdgesNum = 0;

		void OnNodesProcessingComplete();
		void InternalStartExecution();

		FPCGExGraphBuilderDetails BuilderDetails;
		FPCGExBlendingDetails DefaultPointsBlendingDetails;
		FPCGExBlendingDetails DefaultEdgesBlendingDetails;

		TSharedPtr<FGraphBuilder> GraphBuilder;

		FGraphMetadataDetails GraphMetadataDetails;
		TSharedPtr<FPointEdgeIntersections> PointEdgeIntersections;
		TSharedPtr<FEdgeEdgeIntersections> EdgeEdgeIntersections;
		TSharedPtr<PCGExDataBlending::FMetadataBlender> MetadataBlender;


		void FindPointEdgeIntersections();
		void FindPointEdgeIntersectionsFound();
		void OnPointEdgeSortingComplete();
		void OnPointEdgeIntersectionsComplete() const;

		void FindEdgeEdgeIntersections();
		void OnEdgeEdgeIntersectionsFound();
		void OnEdgeEdgeSortingComplete();
		void OnEdgeEdgeIntersectionsComplete() const;
		void CompileFinalGraph();
	};
}