// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Framework/MajulaSubsystem.h"

#include "Algo/MaxElement.h"
#include "Majula/Core/Zone/MajulaZone.h"
#include "Majula/Core/Zone/MajulaZoneRule.h"
#include "Majula/Framework/MajulaManager.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
void UMajulaSubsystem::GetPawnOverlappedZones(const APawn* const& Pawn, TSet<AMajulaZone*>& Zones) const
{
    Zones.Reset();
    Pawn->GetOverlappingActors(reinterpret_cast<TSet<AActor*>&>(Zones), AMajulaZone::StaticClass());
}

AMajulaZone* UMajulaSubsystem::GetPawnPrimaryOverlappedZone(const APawn* const Pawn) const
{
    TSet<AMajulaZone*> Zones;
    GetPawnOverlappedZones(Pawn, Zones);

    if (Manager)
    {
        Algo::Copy(Manager->UnboundZones, Zones);
    }

    TSet<AMajulaZone*> ValidZones;
    Algo::CopyIf(Zones, ValidZones, [Pawn](const AMajulaZone* InZone)
    {
        return InZone && InZone->ValidTest(Pawn);
    });

    const auto* Result = Algo::MaxElement(ValidZones, [](AMajulaZone*& A, AMajulaZone*& B)
    {
        return *A < *B;
    });

    return Result ? *Result : nullptr;
}

ETeamAttitude::Type UMajulaSubsystem::GetAttitudeTowards(const APawn* const& SelfPawn, APawn* const& TargetPawn) const
{
    const auto SelfZone = GetPawnPrimaryOverlappedZone(SelfPawn);
    const auto TargetZone = GetPawnPrimaryOverlappedZone(TargetPawn);

    if (!SelfZone || SelfZone != TargetZone)
    {
        return ETeamAttitude::Neutral;
    }

    const auto ZoneRule = SelfZone->ZoneRule;
    if (!IsValid(ZoneRule))
    {
        return ETeamAttitude::Neutral;
    }

    return SelfZone->ZoneRule.GetDefaultObject()->JudgeAttitude(SelfPawn, TargetPawn);
}


void UMajulaSubsystem::RegisterUnboundZone(AMajulaZone* Zone) const
{
    if (Manager && ensure(Manager->HasAuthority()) && Zone->bUnbound && !Manager->UnboundZones.Contains(Zone))
    {
        Manager->UnboundZones.Add(Zone);
    }
}

void UMajulaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    auto* World = GetWorld();
    if (!World->IsNetMode(NM_Client))
    {
        World->OnWorldBeginPlay.AddWeakLambda(this, [this,World]()
        {
            Manager = World->SpawnActorDeferred<AMajulaManager>(AMajulaManager::StaticClass(), FTransform::Identity,
                                                                nullptr, nullptr,
                                                                ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

            Manager->FinishSpawning(FTransform::Identity);
        });


        FWorldDelegates::OnWorldBeginTearDown.AddWeakLambda(this, [World,this](const UWorld* InWorld)
        {
            if (World == InWorld && IsValid(Manager) && Manager->HasAuthority())
            {
                Manager->Destroy();
            }
        });
    }
}

void UMajulaSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

UMajulaSubsystem* UMajulaSubsystem::Get(const UObject* WorldContextObject)
{
    return WorldContextObject->GetWorld()->GetSubsystem<UMajulaSubsystem>();
}
