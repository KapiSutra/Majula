// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MajulaSubsystem.generated.h"

class AMajulaManager;
class AMajulaZone;
/**
 * 
 */
UCLASS()
class MAJULA_API UMajulaSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    static UMajulaSubsystem* Get(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "Majula")
    void GetActorOverlappedZones(const AActor* const& Actor, TSet<AMajulaZone*>& Zones) const;

    UFUNCTION(BlueprintPure, Category = "Majula")
    AMajulaZone* GetActorPrimaryOverlappedZone(const AActor* const& Actor) const;

    UFUNCTION(BlueprintAuthorityOnly, Category = "Majula")
    void RegisterUnboundZone(AMajulaZone* Zone) const;

    UPROPERTY(Transient)
    TObjectPtr<AMajulaManager> Manager;

#pragma region Overrides
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
#pragma endregion
};
