// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MajulaTeamId.generated.h"

/**
 * FGenericTeamId only support uint8 0~255
 *
 * We use a FGuid to Store TeamId
 */
USTRUCT(BlueprintType)
struct MAJULA_API FMajulaTeamId
{
    GENERATED_BODY()

protected:
    UPROPERTY(Category="MajulaTeamId", BlueprintReadWrite)
    FGuid TeamId;

public:
    FMajulaTeamId()
    {
        TeamId = FGuid();
    };

    explicit FMajulaTeamId(const int& Seed)
    {
        TeamId = FGuid::NewDeterministicGuid(TEXT("Majula"), Seed);
    };

    FMajulaTeamId(const FMajulaTeamId& Other)
    {
        TeamId = Other.TeamId;
    }

    explicit FMajulaTeamId(const FGuid& Guid)
    {
        TeamId = Guid;
    }

    std::weak_ordering operator<=>(const FMajulaTeamId& Other) const
    {
        if (TeamId == Other.TeamId)
        {
            return std::weak_ordering::equivalent;
        }

        if (TeamId < Other.TeamId)
        {
            return std::weak_ordering::less;
        }

        return std::weak_ordering::greater;
    }

    static const FMajulaTeamId NoTeam;

    bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
    {
        Ar << TeamId;
        return bOutSuccess;
    };

    friend FORCEINLINE uint32 GetTypeHash(const FMajulaTeamId& Value)
    {
        return GetTypeHash(Value.TeamId);
    }
};


template <>
struct TStructOpsTypeTraits<FMajulaTeamId> : public TStructOpsTypeTraitsBase2<FMajulaTeamId>
{
    enum
    {
        WithNetSerializer = true,
    };
};
