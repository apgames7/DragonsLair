#include "EnemyAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "NavigationSystem.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AEnemyAIController::OnUnPossess()
{
    Super::OnUnPossess();

    if (GetPathFollowingComponent())
    {
        GetPathFollowingComponent()->AbortMove(
            *this,
            FPathFollowingResultFlags::OwnerFinished
        );
    }
}