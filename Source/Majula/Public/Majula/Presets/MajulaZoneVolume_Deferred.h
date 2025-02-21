// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Majula/Core/Zone/MajulaZoneVolume.h"
#include "MajulaZoneVolume_Deferred.generated.h"

USTRUCT()
struct MAJULA_API FMajulaDeferredZoneDwellSet
{
    GENERATED_BODY()

    int32 Version = 0;

    TSet<TWeakObjectPtr<APawn>> LocalPawns;

    virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
    bool operator==(const FMajulaDeferredZoneDwellSet& Other) const;

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
        WithCopy = true,
        WithIdenticalViaEquality = true,
    };
};

UCLASS()
class MAJULA_API AMajulaZoneVolume_Deferred : public AMajulaZoneVolume
{
    GENERATED_BODY()

public:
    AMajulaZoneVolume_Deferred();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Majula Zone Deferred")
    float DelayTime = 3.f;

    virtual void BeginPlay() override;
    virtual void BeginDestroy() override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool ValidTest_Implementation(const APawn* const Pawn) const override;

protected:
    // Iris Requires a Complicated NetSerializer , See FHitResultNetSerializer
    // UPROPERTY(Replicated)
    // FMajulaDeferredZoneDwellSet DwellSet = FMajulaDeferredZoneDwellSet();

    UPROPERTY(Replicated)
    TArray<TObjectPtr<AActor>> LocalPawns;

    TMap<TWeakObjectPtr<APawn>, FTimerHandle> TimerHandles;

    UFUNCTION(BlueprintAuthorityOnly)
    void HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION(BlueprintAuthorityOnly)
    void HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
