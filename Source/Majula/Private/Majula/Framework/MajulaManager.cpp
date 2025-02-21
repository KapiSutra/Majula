// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Framework/MajulaManager.h"
#include "Majula/Framework/MajulaSubsystem.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMajulaManager::AMajulaManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    bNetLoadOnClient = false;
    bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AMajulaManager::BeginPlay()
{
    Super::BeginPlay();
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
}
