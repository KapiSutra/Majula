// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Presets/MajulaZone_Deferred.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


bool FMajulaDeferredZoneDwellSet::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    auto Count = LocalPawns.Num();
    Ar << Count;

    if (Ar.IsSaving())
    {
        for (auto&& LocalPawn : LocalPawns)
        {
            UObject* PawnPtr = LocalPawn.Get();
            bOutSuccess &= Map->SerializeObject(Ar, LocalPawn->GetClass(), PawnPtr);
        }
    }

    if (Ar.IsLoading())
    {
        while (Count-- > 0)
        {
            UObject* PawnPtr = nullptr;
            bOutSuccess &= Map->SerializeObject(Ar, APawn::StaticClass(), PawnPtr);
            LocalPawns.Add(Cast<APawn>(PawnPtr));
        }
    }

    return bOutSuccess;
}

// Sets default values
AMajulaZone_Deferred::AMajulaZone_Deferred()
{
    PrimaryActorTick.bCanEverTick = false;
    bUnbound = false;
}

// Called when the game starts or when spawned
void AMajulaZone_Deferred::BeginPlay()
{
    Super::BeginPlay();
    check(bUnbound == false);
}

void AMajulaZone_Deferred::BeginDestroy()
{
    Super::BeginDestroy();
    for (auto&& TimerHandle : TimerHandles)
    {
        TimerHandle.Value.Invalidate();
    }
    TimerHandles.Empty();
}

void AMajulaZone_Deferred::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (HasAuthority())
    {
        const auto Pawn = Cast<APawn>(OtherActor);
        if (Pawn)
        {
            FTimerHandle TimerHandle = TimerHandles.FindOrAdd(Pawn);
            GetWorldTimerManager().SetTimer(TimerHandle, [this,Pawn]()
            {
                DwellSet.LocalPawns.Add(Pawn);
                MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DwellSet, this);
            }, DelayTime, false);
        }
    }
}

void AMajulaZone_Deferred::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    if (HasAuthority())
    {
        const auto Pawn = Cast<APawn>(OtherActor);
        if (Pawn)
        {
            auto* PawnHandle = TimerHandles.Find(Pawn);
            if (PawnHandle)
            {
                PawnHandle->Invalidate();
                TimerHandles.Remove(Pawn);
            }
            DwellSet.LocalPawns.Remove(Pawn);
            MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DwellSet, this);
        }
    }
}

void AMajulaZone_Deferred::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    constexpr FDoRepLifetimeParams Params{
        .Condition = COND_OwnerOnly,
        .RepNotifyCondition = REPNOTIFY_OnChanged,
        .bIsPushBased = true
    };
    DOREPLIFETIME_WITH_PARAMS(ThisClass, DwellSet, Params);
}

bool AMajulaZone_Deferred::ValidTest_Implementation(const APawn* const Pawn) const
{
    if (!bEnabled)
    {
        return false;
    }

    return DwellSet.LocalPawns.Contains(Pawn);
}
