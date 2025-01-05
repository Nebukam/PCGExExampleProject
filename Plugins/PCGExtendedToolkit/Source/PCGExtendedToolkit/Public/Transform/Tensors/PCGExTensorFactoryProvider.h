// Copyright 2024 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"
#include "PCGExPointsProcessor.h"
#include "PCGExTensor.h"

#include "PCGExTensorFactoryProvider.generated.h"

#define PCGEX_TENSOR_BOILERPLATE(_TENSOR) \
UPCGExTensorOperation* UPCGExTensor##_TENSOR##Factory::CreateOperation(FPCGExContext* InContext) const{ \
	UPCGExTensor##_TENSOR* NewOperation = InContext->ManagedObjects->New<UPCGExTensor##_TENSOR>(); \
	NewOperation->Config = Config; \
	NewOperation->Config.Init(); \
	NewOperation->BaseConfig = NewOperation->Config; \
	if(!NewOperation->Init(InContext, this)){ return nullptr; } \
	return NewOperation; } \
UPCGExFactoryData* UPCGExCreateTensor##_TENSOR##Settings::CreateFactory(FPCGExContext* InContext, UPCGExFactoryData* InFactory) const{ \
	UPCGExTensor##_TENSOR##Factory* NewFactory = InContext->ManagedObjects->New<UPCGExTensor##_TENSOR##Factory>(); \
	NewFactory->Config = Config; \
	return Super::CreateFactory(InContext, NewFactory);}

class UPCGExTensorOperation;

UCLASS(Abstract, BlueprintType, ClassGroup = (Procedural), Category="PCGEx|Data")
class /*PCGEXTENDEDTOOLKIT_API*/ UPCGExTensorFactoryData : public UPCGExFactoryData
{
	GENERATED_BODY()

	// TODO : To favor re-usability Tensor factories hold more complex logic than regular factories
	// They are also samplers
	// We leverage internal point data and pack all needed attributes & computed points inside

public:
	virtual PCGExFactories::EType GetFactoryType() const override { return PCGExFactories::EType::Tensor; }
	virtual UPCGExTensorOperation* CreateOperation(FPCGExContext* InContext) const;

	virtual bool ExecuteInternal(FPCGExContext* InContext, bool& bAbort) override;

protected:
	virtual bool InitInternalData(FPCGExContext* InContext);
};

UCLASS(Abstract, BlueprintType, ClassGroup = (Procedural), Category="PCGEx|Graph|Params")
class /*PCGEXTENDEDTOOLKIT_API*/ UPCGExTensorFactoryProviderSettings : public UPCGExFactoryProviderSettings
{
	GENERATED_BODY()

public:
	//~Begin UPCGSettings
#if WITH_EDITOR
	PCGEX_NODE_INFOS(Tensor, "Tensor Definition", "Creates a single tensor field definition.")
	virtual FLinearColor GetNodeTitleColor() const override { return GetDefault<UPCGExGlobalSettings>()->NodeColorTensor; }
#endif
	//~End UPCGSettings

	/** Tensor Priority, only accounted for by if sampler is in any Ordered- mode.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable, DisplayPriority=-1))
	int32 Priority;

	virtual FName GetMainOutputPin() const override { return PCGExTensor::OutputTensorLabel; }
	virtual UPCGExFactoryData* CreateFactory(FPCGExContext* InContext, UPCGExFactoryData* InFactory) const override;
};
