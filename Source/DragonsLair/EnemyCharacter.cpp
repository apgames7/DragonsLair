// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "HealthComponent.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

    GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
    GetCapsuleComponent()->SetCapsuleRadius(34.f);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AEnemyAIController::StaticClass();
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = GetPlayerPawn();
    SetState(EEnemyState::Idle);

    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddDynamic(this, &AEnemyCharacter::OnDied);
    }
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HealthComponent && HealthComponent->bIsDead) return;

    if (!PlayerPawn)
        PlayerPawn = GetPlayerPawn();

    if (AttackCooldownRemaining > 0.f)
        AttackCooldownRemaining -= DeltaTime;

    UpdateState(DeltaTime);

#if WITH_EDITOR
    DrawDebugSphere(GetWorld(), GetActorLocation(), DetectionRadius, 24, FColor::Yellow, false, -1.f, 0, 1.f);
    DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRadius, 16, FColor::Red, false, -1.f, 0, 1.f);
#endif
}

// State machine

void AEnemyCharacter::UpdateState(float DeltaTime)
{
    if (!PlayerPawn) return;

    const FVector EnemyLoc = GetActorLocation();
    const FVector PlayerLoc = PlayerPawn->GetActorLocation();
    const float   DistSq = FVector::DistSquared(EnemyLoc, PlayerLoc);
    const float   DetectSq = DetectionRadius * DetectionRadius;
    const float   AttackSq = AttackRadius * AttackRadius;

    switch (CurrentState)
    {
    case EEnemyState::Idle:
        if (DistSq <= DetectSq)
            SetState(EEnemyState::Chase);
        break;

    case EEnemyState::Chase:
        if (DistSq > DetectSq)
            SetState(EEnemyState::Idle);
        else if (DistSq <= AttackSq)
            SetState(EEnemyState::Attack);
        else
        {
            RotateTowardsPlayer(DeltaTime, PlayerLoc);
            AddMovementInput((PlayerLoc - EnemyLoc).GetSafeNormal());
        }
        break;

    case EEnemyState::Attack:
        if (DistSq > AttackSq)
            SetState(EEnemyState::Chase);
        else
        {
            RotateTowardsPlayer(DeltaTime, PlayerLoc);

            if (AttackCooldownRemaining <= 0.f)
            {
                PerformAttack();
                AttackCooldownRemaining = AttackCooldown;
            }
        }
        break;
    }
}

void AEnemyCharacter::SetState(EEnemyState NewState)
{
    if (CurrentState == NewState) return;
    CurrentState = NewState;

    switch (NewState)
    {
    case EEnemyState::Idle:   OnEnterIdle();   break;
    case EEnemyState::Chase:  OnEnterChase();  break;
    case EEnemyState::Attack: OnEnterAttack(); break;
    }
}

// Rotation

void AEnemyCharacter::RotateTowardsPlayer(float DeltaTime, const FVector& PlayerLocation)
{
    const FRotator Current = GetActorRotation();
    const FRotator Target = (PlayerLocation - GetActorLocation()).GetSafeNormal().Rotation();

    const FRotator NewRot = FMath::RInterpTo(
        FRotator(0.f, Current.Yaw, 0.f),
        FRotator(0.f, Target.Yaw, 0.f),
        DeltaTime,
        RotationInterpSpeed
    );

    SetActorRotation(NewRot);
}

// Helpers

APawn* AEnemyCharacter::GetPlayerPawn() const
{
    return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Events

void AEnemyCharacter::OnEnterIdle()
{
    GetCharacterMovement()->StopMovementImmediately();
}

void AEnemyCharacter::OnEnterChase()
{
    // Nothing needed — movement is driven by AddMovementInput in UpdateState
}

void AEnemyCharacter::OnEnterAttack()
{
    GetCharacterMovement()->StopMovementImmediately();
}

void AEnemyCharacter::PerformAttack()
{
    if (!PlayerPawn) return;

    UGameplayStatics::ApplyDamage(
        PlayerPawn,
        AttackDamage,
        GetController(),
        this,
        DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass()
    );
}

void AEnemyCharacter::OnDied(AActor* DeadActor)
{
    GetCharacterMovement()->DisableMovement();
    SetActorEnableCollision(false);
    SetLifeSpan(2.f);
}