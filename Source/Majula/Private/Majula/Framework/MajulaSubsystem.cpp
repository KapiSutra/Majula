// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Framework/MajulaSubsystem.h"

#include "Algo/MaxElement.h"
#include "Majula/Core/Zone/MajulaZoneVolume.h"
#include "Majula/Core/Zone/MajulaZoneRule.h"
#include "Majula/Framework/MajulaManager.h"
#include "Majula/Helpers/MajulaLibrary.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
void UMajulaSubsystem::GetPawnOverlappedZones(const APawn* const& Pawn,
                                              TArray<TScriptInterface<IMajulaZoneInterface>>& Zones) const
{
    Zones.Reset();
    TSet<AActor*> OverlappingActors;
    Pawn->GetOverlappingActors(OverlappingActors);

    for (auto&& OverlappingActor : OverlappingActors)
    {
        if (OverlappingActor->Implements<UMajulaZoneInterface>())
        {
            Zones.Add(TScriptInterface<IMajulaZoneInterface>(OverlappingActor));
        }
    }
}

TScriptInterface<IMajulaZoneInterface> UMajulaSubsystem::GetPawnPrimaryOverlappedZone(const APawn* const Pawn) const
{
    TArray<TScriptInterface<IMajulaZoneInterface>> Zones;
    GetPawnOverlappedZones(Pawn, Zones);

    if (Manager)
    {
        Algo::Copy(Manager->UnboundZones, Zones);
    }

    TArray<TScriptInterface<IMajulaZoneInterface>> ValidZones;
    Algo::CopyIf(Zones, ValidZones, [Pawn](const TScriptInterface<IMajulaZoneInterface>& InZone)
    {
        return InZone.GetObject() && IMajulaZoneInterface::Execute_ValidTest(InZone.GetObject(), Pawn);
    });

    const auto* Result = Algo::MaxElement(
        ValidZones, [&Pawn](const TScriptInterface<IMajulaZoneInterface>& A,
                            const TScriptInterface<IMajulaZoneInterface>& B)
        {
            const auto APriority = IMajulaZoneInterface::Execute_GetZoneContext(A.GetObject()).Priority;
            const auto BPriority = IMajulaZoneInterface::Execute_GetZoneContext(B.GetObject()).Priority;

            if (APriority != BPriority)
            {
                return IMajulaZoneInterface::Execute_GetZoneContext(A.GetObject()).Priority <
                    IMajulaZoneInterface::Execute_GetZoneContext(B.GetObject()).Priority;
            }

            const auto ActorA = UMajulaLibrary::GetZoneActor(A);
            const auto ActorB = UMajulaLibrary::GetZoneActor(B);

            const auto DistanceA = Pawn->GetDistanceTo(ActorA);
            const auto DistanceB = Pawn->GetDistanceTo(ActorB);

            return DistanceA > DistanceB;
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

    if (!SelfZone.GetObject())
    {
        return ETeamAttitude::Neutral;
    }

    const auto ZoneRule = IMajulaZoneInterface::Execute_GetZoneContext(SelfZone.GetObject()).ZoneRule;
    if (!IsValid(ZoneRule))
    {
        return ETeamAttitude::Neutral;
    }

    return ZoneRule.GetDefaultObject()->JudgeAttitude(SelfPawn, TargetPawn);
}


void UMajulaSubsystem::RegisterUnboundZone(const TScriptInterface<IMajulaZoneInterface>& Zone) const
{
    // if (!IsValid(Manager))
    // {
    //     GetWorld()->GetTimerManager().SetTimerForNextTick([this,Zone]()
    //     {
    //         RegisterUnboundZone(Zone);
    //     });
    //     return;
    // }

    const auto ZoneContext = IMajulaZoneInterface::Execute_GetZoneContext(Zone.GetObject());

    if (Manager && ensure(Manager->HasAuthority()) && ZoneContext.bUnbound && !Manager->UnboundZones.Contains(Zone))
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
        World->OnWorldPreBeginPlay.AddWeakLambda(this, [this,World]()
        {
            Manager = World->SpawnActor<AMajulaManager>(AMajulaManager::StaticClass());
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
