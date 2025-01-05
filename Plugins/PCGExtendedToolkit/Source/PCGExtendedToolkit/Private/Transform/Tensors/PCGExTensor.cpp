// Copyright 2024 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/

#include "Transform/Tensors/PCGExTensor.h"

#include "Transform/Tensors/PCGExTensorFactoryProvider.h"
#include "Transform/Tensors/PCGExTensorOperation.h"

namespace PCGExTensor
{
	FTensorsHandler::FTensorsHandler()
	{
	}

	bool FTensorsHandler::Init(FPCGExContext* InContext, const TArray<TObjectPtr<const UPCGExTensorFactoryData>>& InFactories)
	{
		Operations.Reserve(InFactories.Num());

		for (const UPCGExTensorFactoryData* Factory : InFactories)
		{
			UPCGExTensorOperation* Op = Factory->CreateOperation(InContext);
			Operations.Add(Op);
		}

		return true;
	}

	bool FTensorsHandler::Init(FPCGExContext* InContext, const FName InPin)
	{
		TArray<TObjectPtr<const UPCGExTensorFactoryData>> InFactories;
		if (!PCGExFactories::GetInputFactories(InContext, InPin, InFactories, {PCGExFactories::EType::Tensor}, true)) { return false; }
		return Init(InContext, InFactories);
	}

	bool FTensorsHandler::SamplePosition(const FVector& InPosition, FTensorSample& OutSample)
	{
		// TODO : Go through all operations and gather samples, weighted average
		return false;
	}

	bool FTensorsHandler::SamplePositionOrderedInPlace(const FVector& InPosition, FTensorSample& OutSample)
	{
		// TODO : Go through all operations and gather samples, apply them one after another
		return false;
	}

	bool FTensorsHandler::SamplePositionOrderedMutated(const FVector& InPosition, FTensorSample& OutSample)
	{
		FVector UpdatedPosition = InPosition;
		// TODO : Go through all operations and gather samples, apply them & update sampling position one after another
		return false;
	}
}
