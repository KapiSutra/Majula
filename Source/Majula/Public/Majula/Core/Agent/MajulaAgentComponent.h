// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MajulaAgentComponent.generated.h"

class UMajulaAgentStrategy;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMajulaAgentStrategyChanged);

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent), ClassGroup="Majula")
class MAJULA_API UMajulaAgentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMajulaAgentComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    //
    UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing="OnRep_TeamStrategy", Category="Majula", Instanced)
    TObjectPtr<UMajulaAgentStrategy> TeamStrategy;

    UFUNCTION()
    virtual void OnRep_TeamStrategy()
    {
    };

    UFUNCTION(BlueprintCallable, Category = "Majula", BlueprintAuthorityOnly)
    void SetTeamStrategy(UMajulaAgentStrategy* InTeamStrategy);

    UPROPERTY(BlueprintAssignable, Category="Majula", BlueprintAuthorityOnly)
    FMajulaAgentStrategyChanged PostMajulaAgentStrategyChanged;

protected:
    UFUNCTION()
    void HandlePawnChanged(APawn* OldPawn, APawn* NewPawn);

    UPROPERTY(Transient)
    TObjectPtr<APawn> Avatar;
};
