// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Core/Zone/MajulaZone.h"

#include "Components/BrushComponent.h"
#include "Majula/Framework/MajulaSubsystem.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMajulaZone::AMajulaZone()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    bEnabled = true;
    bUnbound = 0;

    auto* CollisionBrush = GetBrushComponent();
    CollisionBrush->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
    CollisionBrush->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
    CollisionBrush->SetCollisionObjectType(ECC_WorldStatic);
    CollisionBrush->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBrush->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionBrush->Mobility = EComponentMobility::Static;

    bReplicates = true;
    bNetLoadOnClient = true;
    SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

// Called when the game starts or when spawned
void AMajulaZone::BeginPlay()
{
    Super::BeginPlay();


    if (HasAuthority())
    {
        if (bUnbound)
        {
            UMajulaSubsystem::Get(this)->RegisterUnboundZone(this);
        }
    }
}


void AMajulaZone::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    constexpr FDoRepLifetimeParams Params{
        .Condition = COND_SimulatedOnly,
        .RepNotifyCondition = REPNOTIFY_OnChanged,
        .bIsPushBased = true,
    };
    DOREPLIFETIME_WITH_PARAMS(ThisClass, bEnabled, Params);
    DOREPLIFETIME_WITH_PARAMS(ThisClass, bUnbound, Params);
    DOREPLIFETIME_WITH_PARAMS(ThisClass, Priority, Params);
    DOREPLIFETIME_WITH_PARAMS(ThisClass, ZoneRule, Params);
}


auto AMajulaZone::operator<=>(const AMajulaZone& Other) const
{
    return Priority <=> Other.Priority;
}

void AMajulaZone::PostNetReceive()
{
    Super::PostNetReceive();
}

bool AMajulaZone::ValidTest_Implementation(const APawn* const Pawn) const
{
    return bEnabled;
}


#if WITH_EDITOR
void AMajulaZone::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
