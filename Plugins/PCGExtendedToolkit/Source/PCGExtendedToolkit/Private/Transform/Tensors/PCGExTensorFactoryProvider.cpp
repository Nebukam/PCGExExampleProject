﻿// Copyright 2024 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/

#include "Transform/Tensors/PCGExTensorFactoryProvider.h"

#include "Paths/PCGExPaths.h"
#include "Paths/PCGExSplineToPath.h"
#include "Transform/Tensors/PCGExTensorOperation.h"

#define LOCTEXT_NAMESPACE "PCGExCreateTensor"
#define PCGEX_NAMESPACE CreateTensor

UPCGExTensorOperation* UPCGExTensorFactoryData::CreateOperation(FPCGExContext* InContext) const
{
	return nullptr; // Create shape builder operation
}

bool UPCGExTensorFactoryData::Prepare(FPCGExContext* InContext)
{
	if (!Super::Prepare(InContext)) { return false; }
	return InitInternalData(InContext);
}

bool UPCGExTensorFactoryData::InitInternalData(FPCGExContext* InContext)
{
	return true;
}

UPCGExFactoryData* UPCGExTensorFactoryProviderSettings::CreateFactory(FPCGExContext* InContext, UPCGExFactoryData* InFactory) const
{
	return InFactory;
}

bool UPCGExTensorPointFactoryData::InitInternalData(FPCGExContext* InContext)
{
	if (!Super::InitInternalData(InContext)) { return false; }
	if (!InitInternalFacade(InContext)) { return false; }

	GetMutablePoints() = InputDataFacade->GetIn()->GetPoints();

	FBox InBounds = InputDataFacade->GetIn()->GetBounds();
	TOctree2<FPCGPointRef, FPCGPointRefSemantics> NewOctree(InBounds.GetCenter(), InBounds.GetExtent().Length());

	// Pack per-point data
	for (int i = 0; i < Points.Num(); i++)
	{
		FPCGPoint& Effector = Points[i];
		PrepareSinglePoint(i, Effector);

		Effector.MetadataEntry = PCGInvalidEntryKey;

		// Flatten bounds

		FBox ScaledBounds = PCGExMath::GetLocalBounds<EPCGExPointBoundsSource::ScaledBounds>(Effector);
		FBox WorldBounds = ScaledBounds.TransformBy(Effector.Transform);
		FVector Extents = ScaledBounds.GetExtent();

		Effector.BoundsMin = Extents * -1;
		Effector.BoundsMax = Extents;

		// Insert to octree with desired large bounds
		NewOctree.AddElement(FPCGPointRef(Effector));

		// Flatten original bounds
		Effector.Transform.SetLocation(WorldBounds.GetCenter());
		Effector.Transform.SetScale3D(FVector::OneVector);

		Effector.BoundsMin = ScaledBounds.Min;
		Effector.BoundsMax = ScaledBounds.Max;

		Effector.Color = FVector4(Extents.X, Extents.Y, Extents.Z, Extents.SquaredLength()); // Cache Scaled Extents + Squared radius into $Color
		Effector.Density = Effector.Steepness;                                               // Pack Weight to $Density
		Effector.Steepness = StrengthBuffer->Read(i);                                        // Pack Strength to $Steepness
	}

	bOctreeIsDirty = false;
	Octree = MoveTemp(NewOctree);

	InputDataFacade->Flush(); // Flush cached buffers
	InputDataFacade.Reset();

	return true;
}

bool UPCGExTensorPointFactoryData::InitInternalFacade(FPCGExContext* InContext)
{
	InputDataFacade = PCGExData::TryGetSingleFacade(InContext, PCGEx::SourcePointsLabel, true);
	if (!InputDataFacade) { return false; }

	StrengthBuffer = InputDataFacade->GetBroadcaster<float>(BaseConfig.StrengthAttribute, true);
	if (!StrengthBuffer)
	{
		PCGE_LOG_C(Error, GraphAndLog, InContext, FText::Format(FTEXT("Invalid Strength attribute: \"{0}\"."), FText::FromName(BaseConfig.StrengthAttribute.GetName())));
		return false;
	}

	return true;
}

void UPCGExTensorPointFactoryData::PrepareSinglePoint(const int32 Index, FPCGPoint& InPoint) const
{
}

TArray<FPCGPinProperties> UPCGExTensorPointFactoryProviderSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties = Super::InputPinProperties();
	PCGEX_PIN_POINT(PCGEx::SourcePointsLabel, "Single point collection that represent attractors", Required, {})
	return PinProperties;
}

#undef LOCTEXT_NAMESPACE
#undef PCGEX_NAMESPACE