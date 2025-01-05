// Copyright 2024 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/

#include "Transform/Tensors/PCGExTensorFlow.h"

#define LOCTEXT_NAMESPACE "PCGExCreateTensorFlow"
#define PCGEX_NAMESPACE CreateTensorFlow

bool UPCGExTensorFlow::Init(FPCGExContext* InContext, const UPCGExTensorFactoryData* InFactory)
{
	if (!Super::Init(InContext, InFactory)) { return false; }
	return true;
}

PCGEX_TENSOR_BOILERPLATE(Flow)

bool UPCGExTensorFlowFactory::InitInternalData(FPCGExContext* InContext)
{
	if (!Super::InitInternalData(InContext)) { return false; }

	TSharedPtr<PCGExData::FPointIO> InPoints = PCGExData::TryGetSingleInput(InContext, PCGEx::SourcePointsLabel, true);
	if (!InPoints) { return false; }

	GetMutablePoints() = InPoints->GetPoints(PCGExData::ESource::In);
	return true;
}

TArray<FPCGPinProperties> UPCGExCreateTensorFlowSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties = Super::InputPinProperties();
	PCGEX_PIN_POINT(PCGEx::SourcePointsLabel, "Single point collection whose transform represent the flow", Required, {})
	return PinProperties;
}

#undef LOCTEXT_NAMESPACE
#undef PCGEX_NAMESPACE
