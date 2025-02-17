// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Framework/MajulaSubsystem.h"

#include "Algo/MaxElement.h"
#include "Majula/Core/Zone/MajulaZone.h"
#include "Majula/Framework/MajulaManager.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
void UMajulaSubsystem::GetActorOverlappedZones(const AActor* const& Actor, TSet<AMajulaZone*>& Zones) const
{
    Zones.Reset();
    Actor->GetOverlappingActors(reinterpret_cast<TSet<AActor*>&>(Zones), AMajulaZone::StaticClass());
}

AMajulaZone* UMajulaSubsystem::GetActorPrimaryOverlappedZone(const AActor* const& Actor) const
{
    TSet<AMajulaZone*> Zones;
    GetActorOverlappedZones(Actor, Zones);

    TSet<AMajulaZone*> AllZones;
    if (Manager)
    {
        Algo::Copy(Manager->UnboundZones, AllZones);
    }
    Algo::Copy(Zones, AllZones);

    const auto* Result = Algo::MaxElement(AllZones, [](AMajulaZone*& A, AMajulaZone*& B)
    {
        return *A < *B;
    });

    return Result ? *Result : nullptr;
}

void UMajulaSubsystem::RegisterUnboundZone(AMajulaZone* Zone) const
{
    if (Manager && ensure(Manager->HasAuthority()) && Zone->bUnbound)
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
