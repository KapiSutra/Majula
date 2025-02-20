// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MajulaZoneInterface.generated.h"

class UMajulaZoneRule;

USTRUCT(BlueprintType)
struct MAJULA_API FMajulaZoneContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="MajulaZone")
    int32 Priority = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="MajulaZone")
    uint8 bUnbound : 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="MajulaZone", meta=(AllowAbstract=false))
    TSubclassOf<UMajulaZoneRule> ZoneRule;
};

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UMajulaZoneInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class MAJULA_API IMajulaZoneInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintNativeEvent, Category = "MajulaZone", BlueprintCallable)
    FMajulaZoneContext GetZoneContext() const;

    UFUNCTION(BlueprintNativeEvent, Category = "MajulaZone")
    bool ValidTest(const APawn* const Pawn) const;
};
