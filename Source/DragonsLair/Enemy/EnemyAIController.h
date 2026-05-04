#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class DRAGONSLAIR_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;;
};