// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MajulaTypes.generated.h"

class AMajulaZoneVolume;

USTRUCT(BlueprintType)
struct MAJULA_API FMajulaZoneData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TSubclassOf<AMajulaZoneVolume> ZoneClass;
};
