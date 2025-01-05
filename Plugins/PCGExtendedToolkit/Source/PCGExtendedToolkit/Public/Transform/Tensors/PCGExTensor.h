// Copyright 2024 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once
#include "PCGExDetails.h"
#include "Data/PCGExData.h"
#include "PCGExTensor.generated.h"

class UPCGExTensorFactoryData;
class UPCGExTensorOperation;

UENUM()
enum class EPCGExTensorSamplingMode : uint8
{
	Weighted       = 0 UMETA(DisplayName = "Weighted", ToolTip="Compute a weighted average of the sampled tensors"),
	OrderedInPlace = 1 UMETA(DisplayName = "Ordered (in place)", ToolTip="Applies tensor one after another in order, using the same original position"),
	OrderedMutated = 2 UMETA(DisplayName = "Ordered (mutated)", ToolTip="Applies tensor & update sampling position one after another in order"),
};

USTRUCT(BlueprintType)
struct /*PCGEXTENDEDTOOLKIT_API*/ FPCGExTensorSamplingDetails
{
	GENERATED_BODY()

	FPCGExTensorSamplingDetails()
	{
	}

	virtual ~FPCGExTensorSamplingDetails()
	{
	}

	/** Resolution input type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Weighting", meta = (PCG_NotOverridable))
	EPCGExTensorSamplingMode SamplingMode = EPCGExTensorSamplingMode::Weighted;
};

USTRUCT(BlueprintType)
struct /*PCGEXTENDEDTOOLKIT_API*/ FPCGExTensorConfigBase
{
	GENERATED_BODY()

	FPCGExTensorConfigBase()
	{
	}

	virtual ~FPCGExTensorConfigBase()
	{
	}

	/** Resolution input type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Weighting", meta = (PCG_NotOverridable))
	EPCGExInputValueType WeightInput = EPCGExInputValueType::Constant;

	/** Resolution Constant. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Weighting", meta=(PCG_Overridable, DisplayName="Weight", EditCondition="WeightInput == EPCGExInputValueType::Constant", EditConditionHides, ClampMin=0))
	double ResolutionConstant = 1;

	/** Resolution Attribute. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Weighting", meta=(PCG_Overridable, DisplayName="Weight", EditCondition="WeightInput == EPCGExInputValueType::Attribute", EditConditionHides))
	FPCGAttributePropertyInputSelector WeightAttribute;

	/** Uniform weight factor for this tensor. Multiplier applied to individual output values. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Weighting", meta=(PCG_Overridable, EditCondition="ResolutionInput == EPCGExInputValueType::Constant"))
	double UniformWeightFactor = 1;

	virtual void Init()
	{
	}
};

namespace PCGExTensor
{
	const FName OutputTensorLabel = TEXT("Tensor");
	const FName SourceTensorsLabel = TEXT("Tensors");

	struct FTensorSample
	{
		FVector Direction = FVector::Zero(); // sample direction
		double Strength = 0;                 // i.e, length
		int32 Effectors = 0;                 // number of things that affected this sample
		double TotalWeight = 0;              // total weights applied to this sample

		FTensorSample() = default;
		~FTensorSample() = default;
	};

	class FTensorsHandler : public TSharedFromThis<FTensorsHandler>
	{
		TArray<UPCGExTensorOperation*> Operations;

	public:
		FTensorsHandler();
		bool Init(FPCGExContext* InContext, const TArray<TObjectPtr<const UPCGExTensorFactoryData>>& InFactories);
		bool Init(FPCGExContext* InContext, const FName InPin);

		bool SamplePosition(const FVector& InPosition, FTensorSample& OutSample);
		bool SamplePositionOrderedInPlace(const FVector& InPosition, FTensorSample& OutSample);
		bool SamplePositionOrderedMutated(const FVector& InPosition, FTensorSample& OutSample);
	};
}
