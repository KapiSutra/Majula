// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Majula/Core/Team/MajulaTeamId.h"
#include "Net/Core/PushModel/PushModelMacros.h"
#include "UObject/Object.h"
#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif
#include "MajulaAgentStrategy.generated.h"

struct FGenericTeamId;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class MAJULA_API UMajulaAgentStrategy : public UObject
{
    GENERATED_BODY()
    REPLICATED_BASE_CLASS()

public:
    virtual bool IsSupportedForNetworking() const override { return true; };
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
#if UE_WITH_IRIS
    virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
                                              const UE::Net::EFragmentRegistrationFlags RegistrationFlags) override
    {
        UE::Net::FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
    };
#endif


    UFUNCTION(BlueprintNativeEvent, Category="Majula", BlueprintPure)
    FMajulaTeamId ResolveTeamId(const AController* const Controller) const;
};
