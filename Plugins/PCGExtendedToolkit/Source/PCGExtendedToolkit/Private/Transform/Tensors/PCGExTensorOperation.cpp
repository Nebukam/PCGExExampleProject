// Copyright 2024 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/


#include "Transform/Tensors/PCGExTensorOperation.h"
#include "Transform/Tensors/PCGExTensorFactoryProvider.h"

void UPCGExTensorOperation::CopySettingsFrom(const UPCGExOperation* Other)
{
	Super::CopySettingsFrom(Other);
	//if (const UPCGExTensorOperation* TypedOther = Cast<UPCGExTensorOperation>(Other))	{	}
}

bool UPCGExTensorOperation::Init(FPCGExContext* InContext, const UPCGExTensorFactoryData* InFactory)
{
	Factory = InFactory;
	return true;
}

bool UPCGExTensorOperation::SamplePosition(const FVector& InPosition, PCGExTensor::FTensorSample& OutSample)
{
	return false;
}

void UPCGExTensorPointOperation::CopySettingsFrom(const UPCGExOperation* Other)
{
	Super::CopySettingsFrom(Other);
	//if (const UPCGExTensorPointOperation* TypedOther = Cast<UPCGExTensorPointOperation>(Other))	{	}
}

bool UPCGExTensorPointOperation::Init(FPCGExContext* InContext, const UPCGExTensorFactoryData* InFactory)
{
	if (!Super::Init(InContext, InFactory)) { return false; }
	Octree = &InFactory->GetOctree();
	return true;
}
