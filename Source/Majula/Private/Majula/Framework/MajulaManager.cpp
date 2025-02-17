// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Framework/MajulaManager.h"

#include "Majula/Core/Zone/MajulaZone.h"
#include "Majula/Framework/MajulaSubsystem.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMajulaManager::AMajulaManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    bNetLoadOnClient = false;
}

// Called when the game starts or when spawned
void AMajulaManager::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        FallbackZone = GetWorld()->SpawnActorDeferred<AMajulaZone>(AMajulaZone::StaticClass(), FTransform::Identity,
                                                                   this, nullptr,
                                                                   ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
        FallbackZone->bUnbound = true;
        FallbackZone->Priority = TNumericLimits<int>::Min();
        FallbackZone->Rename(TEXT("MajulaFallbackZone"), this->GetOuter());
        FallbackZone->FinishSpawning(FTransform::Identity);
    }
}

void AMajulaManager::PostNetReceive()
{
    Super::PostNetReceive();
    UMajulaSubsystem::Get(this)->Manager = this;
}

void AMajulaManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    constexpr FDoRepLifetimeParams Params{
        .Condition = COND_None,
        .RepNotifyCondition = REPNOTIFY_OnChanged,
        .bIsPushBased = true
    };
    DOREPLIFETIME_WITH_PARAMS(ThisClass, UnboundZones, Params);
    DOREPLIFETIME_WITH_PARAMS(ThisClass, FallbackZone, Params);
}
