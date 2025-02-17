// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif

#include "MajulaZone.generated.h"

class UMajulaZoneRule;

UCLASS()
class MAJULA_API AMajulaZone : public AVolume
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMajulaZone();
    auto operator<=>(const AMajulaZone&) const;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
#if UE_WITH_IRIS
    virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
                                              UE::Net::EFragmentRegistrationFlags RegistrationFlags) override
    {
        UE::Net::FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
    };
#endif

    virtual void PostNetReceive() override;

public:
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="MajulaZone"
        , Replicated
    )
    int32 Priority = 0;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="MajulaZone"
        , Replicated
    )
    uint8 bEnabled : 1;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="MajulaZone"
        , Replicated
    )
    uint8 bUnbound : 1;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="MajulaZone")
    TSubclassOf<UMajulaZoneRule> ZoneRule;
};
