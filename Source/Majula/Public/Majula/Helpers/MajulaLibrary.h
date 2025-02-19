// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Majula/Core/Team/MajulaTeamId.h"
#include "MajulaLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MAJULA_API UMajulaLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure,
        meta=(DisplayName = "Equal (Majula TeamId)", CompactNodeTitle = "==", Keywords = "== equal"),
        Category="Utilities")
    static FORCEINLINE bool EqualEqual_MajulaTeam(const FMajulaTeamId& A,
                                                  const FMajulaTeamId& B)
    {
        return A <=> B == std::weak_ordering::equivalent;
    };

    UFUNCTION(BlueprintPure, Category="Majula", DisplayName="Get NoTeam Id")
    static FORCEINLINE FMajulaTeamId GetNoTeamId()
    {
        return FMajulaTeamId::NoTeam;
    }
};
