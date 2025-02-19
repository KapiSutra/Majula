// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Public/Majula/Core/Agent/MajulaAgentComponent.h"

#include "Net/UnrealNetwork.h"

UMajulaAgentComponent::UMajulaAgentComponent()
{
}

void UMajulaAgentComponent::BeginPlay()
{
    Super::BeginPlay();

    // Must Attach to Controller
    auto* Controller = Cast<AController>(GetOwner());
    if (ensure(Controller))
    {
        APawn* Pawn = Controller->GetPawn();
        if (Pawn)
        {
            HandlePawnChanged(nullptr, Pawn);
        }
        Controller->OnPossessedPawnChanged.AddDynamic(this, &UMajulaAgentComponent::HandlePawnChanged);
    }
}

void UMajulaAgentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    FDoRepLifetimeParams Params;
    Params.bIsPushBased = true;
    Params.Condition = COND_OwnerOnly;
    DOREPLIFETIME_WITH_PARAMS(ThisClass, TeamStrategy, Params);
}

void UMajulaAgentComponent::SetTeamStrategy(UMajulaAgentStrategy* InTeamStrategy)
{
    TeamStrategy = InTeamStrategy;
    PostMajulaAgentStrategyChanged.Broadcast();
}

void UMajulaAgentComponent::HandlePawnChanged(APawn* OldPawn, APawn* NewPawn)
{
    Avatar = NewPawn;
}
