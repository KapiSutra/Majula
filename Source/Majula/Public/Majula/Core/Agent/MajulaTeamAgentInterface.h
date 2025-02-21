// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MajulaTeamAgentInterface.generated.h"

struct FMajulaTeamId;
class UMajulaAgentStrategy;
class UMajulaAgentComponent;
// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable)
class UMajulaTeamAgentInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class MAJULA_API IMajulaTeamAgentInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintCallable, Category="Majula")
    virtual UMajulaAgentComponent* GetTeamAgentComponent() const;

    UFUNCTION(BlueprintCallable, Category="Majula")
    virtual UMajulaAgentStrategy* GetAgentStrategy() const;

    UFUNCTION(BlueprintCallable, Category="Majula", BlueprintAuthorityOnly)
    virtual void SetAgentStrategy(UMajulaAgentStrategy* AgentStrategy);
};
