// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Majula/Core/Zone/MajulaZone.h"
#include "MajulaZone_Deferred.generated.h"

USTRUCT()
struct MAJULA_API FMajulaDeferredZoneDwellSet
{
    GENERATED_BODY()

    TSet<TWeakObjectPtr<APawn>> LocalPawns;

    bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);

    virtual ~FMajulaDeferredZoneDwellSet()
    {
        LocalPawns.Empty();
    };
};

template <>
struct TStructOpsTypeTraits<FMajulaDeferredZoneDwellSet> : TStructOpsTypeTraitsBase2<FMajulaDeferredZoneDwellSet>
{
    enum
    {
        WithNetSerializer = true,
    };
};

UCLASS()
class MAJULA_API AMajulaZone_Deferred : public AMajulaZone
{
    GENERATED_BODY()

public:
    AMajulaZone_Deferred();

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
    float DelayTime = 5.f;

protected:
    virtual void BeginPlay() override;
    virtual void BeginDestroy() override;

public:
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    virtual bool ValidTest_Implementation(const APawn* const Pawn) const override;

protected:
    UPROPERTY(Replicated)
    FMajulaDeferredZoneDwellSet DwellSet;

    TMap<TWeakObjectPtr<APawn>, FTimerHandle> TimerHandles;
};
