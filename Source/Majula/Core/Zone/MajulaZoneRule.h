// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "UObject/Object.h"
#include "MajulaZoneRule.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, meta=(DontUseGenericSpawnObject=true))
class MAJULA_API UMajulaZoneRule : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category="Majula")
    ETeamAttitude::Type JudgeAttitude(const APawn* const SelfPawn,
                                      const APawn* const TargetPawn);
};
