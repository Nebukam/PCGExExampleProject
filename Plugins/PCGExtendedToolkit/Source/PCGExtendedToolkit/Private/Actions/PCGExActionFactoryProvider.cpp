﻿// Copyright 2024 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/

#include "Actions/PCGExActionFactoryProvider.h"

#include "PCGPin.h"


#define LOCTEXT_NAMESPACE "PCGExWriteActions"
#define PCGEX_NAMESPACE PCGExWriteActions


void UPCGExActionOperation::CopySettingsFrom(const UPCGExOperation* Other)
{
	Super::CopySettingsFrom(Other);
	if (const UPCGExActionOperation* TypedOther = Cast<UPCGExActionOperation>(Other))
	{
		Factory = TypedOther->Factory;
	}
}

bool UPCGExActionOperation::PrepareForData(FPCGExContext* InContext, const TSharedPtr<PCGExData::FFacade>& InPointDataFacade)
{
	PrimaryDataFacade = InPointDataFacade;

	FilterManager = MakeShared<PCGExPointFilter::FManager>(PrimaryDataFacade.ToSharedRef());

	if (!FilterManager->Init(InContext, Factory->FilterFactories)) { return false; }

	return true;
}

void UPCGExActionOperation::ProcessPoint(const int32 Index, const FPCGPoint& Point)
{
	if (FilterManager->Test(Index)) { OnMatchSuccess(Index, Point); }
	else { OnMatchFail(Index, Point); }
}

void UPCGExActionOperation::OnMatchSuccess(int32 Index, const FPCGPoint& Point)
{
}

void UPCGExActionOperation::OnMatchFail(int32 Index, const FPCGPoint& Point)
{
}

void UPCGExActionOperation::Cleanup()
{
	Super::Cleanup();
}

#if WITH_EDITOR
FString UPCGExActionProviderSettings::GetDisplayName() const { return TEXT(""); }
#endif

UPCGExActionOperation* UPCGExActionFactoryBase::CreateOperation(FPCGExContext* InContext) const
{
	UPCGExActionOperation* NewOperation = InContext->ManagedObjects->New<UPCGExActionOperation>();
	NewOperation->Factory = const_cast<UPCGExActionFactoryBase*>(this);
	return NewOperation;
}

bool UPCGExActionFactoryBase::Boot(FPCGContext* InContext)
{
	return true;
}

bool UPCGExActionFactoryBase::AppendAndValidate(PCGEx::FAttributesInfos* InInfos, FString& OutMessage) const
{
	TSet<FName> Mismatch;

	const FPCGExAttributeGatherDetails GatherDetails = FPCGExAttributeGatherDetails(); // Required for Append

	if (CheckSuccessInfos) { InInfos->Append(CheckSuccessInfos, GatherDetails, Mismatch); }
	if (!Mismatch.IsEmpty())
	{
		for (const FName& MismatchName : Mismatch)
		{
			OutMessage = FText::Format(FText::FromString(TEXT("Attribute \"{0}\" is referenced multiple times but has different types.")), FText::FromName(MismatchName)).ToString();
		}
		return false;
	}

	if (CheckFailInfos) { InInfos->Append(CheckFailInfos, GatherDetails, Mismatch); }
	if (!Mismatch.IsEmpty())
	{
		for (const FName& MismatchName : Mismatch)
		{
			OutMessage = FText::Format(FText::FromString(TEXT("Attribute \"{0}\" is referenced multiple times but has different types.")), FText::FromName(MismatchName)).ToString();
		}
		return false;
	}

	return true;
}

void UPCGExActionFactoryBase::BeginDestroy()
{
	Super::BeginDestroy();
}

TArray<FPCGPinProperties> UPCGExActionProviderSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties = Super::InputPinProperties();
	if (GetRequiresFilters()) { PCGEX_PIN_PARAMS(PCGExActions::SourceConditionsFilterLabel, "Filters used to define if there's a match or not.", Required, {}) }
	else { PCGEX_PIN_PARAMS(PCGExActions::SourceConditionsFilterLabel, "Filters used to define if there's a match or not.", Normal, {}) }
	return PinProperties;
}

UPCGExParamFactoryBase* UPCGExActionProviderSettings::CreateFactory(FPCGExContext* InContext, UPCGExParamFactoryBase* InFactory) const
{
	if (UPCGExActionFactoryBase* TypedFactory = Cast<UPCGExActionFactoryBase>(InFactory))
	{
		if (!GetInputFactories(
			InContext, PCGExActions::SourceConditionsFilterLabel, TypedFactory->FilterFactories,
			PCGExFactories::PointFilters, true))
		{
			return nullptr;
		}

		TypedFactory->Priority = Priority;

		if (!TypedFactory->Boot(InContext)) { return nullptr; }
	}
	else
	{
		return nullptr;
	}

	return InFactory;
}


#undef LOCTEXT_NAMESPACE
#undef PCGEX_NAMESPACE