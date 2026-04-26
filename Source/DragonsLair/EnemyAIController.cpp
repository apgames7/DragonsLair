// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Navigation/CrowdFollowingComponent.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
    bAttachToPawn = true;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AEnemyAIController::OnUnPossess()
{
    Super::OnUnPossess();
    StopMovement();
}