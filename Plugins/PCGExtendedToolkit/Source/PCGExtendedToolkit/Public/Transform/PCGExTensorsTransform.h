﻿// Copyright 2024 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"
#include "PCGEx.h"
#include "PCGExGlobalSettings.h"

#include "PCGExPointsProcessor.h"
#include "PCGExTransform.h"
#include "Tensors/PCGExTensor.h"


#include "PCGExTensorsTransform.generated.h"


UCLASS(BlueprintType, ClassGroup = (Procedural), Category="PCGEx|Misc")
class /*PCGEXTENDEDTOOLKIT_API*/ UPCGExTensorsTransformSettings : public UPCGExPointsProcessorSettings
{
	GENERATED_BODY()

public:
	//~Begin UPCGSettings
#if WITH_EDITOR
	PCGEX_NODE_INFOS(TensorsTransform, "Tensors Transform", "Transform input points using tensors.");
	virtual FLinearColor GetNodeTitleColor() const override { return GetDefault<UPCGExGlobalSettings>()->NodeColorTransform; }
#endif

protected:
	virtual TArray<FPCGPinProperties> InputPinProperties() const override;
	virtual FPCGElementPtr CreateElement() const override;
	//~End UPCGSettings

	//~Begin UPCGExPointsProcessorSettings
public:
	virtual PCGExData::EIOInit GetMainOutputInitMode() const override;
	PCGEX_NODE_POINT_FILTER(PCGExPointFilter::SourceFiltersLabel, "Filters", PCGExFactories::PointFilters, false)
	//~End UPCGExPointsProcessorSettings

	/**  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(ShowOnlyInnerProperties))
	FPCGExTensorSamplingDetails SamplingDetails;


	/**  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable))
	bool bTransformPosition = true;

	/**  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bTransformRotation = true;

	/**  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable, EditCondition="bTransformRotation"))
	EPCGExTransformMode Rotation = EPCGExTransformMode::Absolute;

	/**  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable))
	int32 Iterations = 1;

private:
	friend class FPCGExTensorsTransformElement;
};

struct /*PCGEXTENDEDTOOLKIT_API*/ FPCGExTensorsTransformContext final : FPCGExPointsProcessorContext
{
	friend class FPCGExTensorsTransformElement;
	TSharedPtr<PCGExTensor::FTensorsHandler> TensorsHandler;
};

class /*PCGEXTENDEDTOOLKIT_API*/ FPCGExTensorsTransformElement final : public FPCGExPointsProcessorElement
{
	virtual FPCGContext* Initialize(
		const FPCGDataCollection& InputData,
		TWeakObjectPtr<UPCGComponent> SourceComponent,
		const UPCGNode* Node) override;

protected:
	virtual bool Boot(FPCGExContext* InContext) const override;
	virtual bool ExecuteInternal(FPCGContext* Context) const override;
};

namespace PCGExTensorsTransform
{
	class FProcessor final : public PCGExPointsMT::TPointsProcessor<FPCGExTensorsTransformContext, UPCGExTensorsTransformSettings>
	{
		bool bIteratedOnce = false;
		int32 RemainingIterations = 0;

	public:
		explicit FProcessor(const TSharedRef<PCGExData::FFacade>& InPointDataFacade):
			TPointsProcessor(InPointDataFacade)
		{
		}

		virtual ~FProcessor() override;

		virtual bool Process(const TSharedPtr<PCGExMT::FTaskManager> InAsyncManager) override;
		virtual void PrepareSingleLoopScopeForPoints(const PCGExMT::FScope& Scope) override;
		virtual void ProcessSinglePoint(const int32 Index, FPCGPoint& Point, const PCGExMT::FScope& Scope) override;
		virtual void OnPointsProcessingComplete() override;
	};
}