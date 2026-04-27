#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "HealthComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

AEnemyCharacter::AEnemyCharacter()
{
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

void AEnemyCharacter::UpdateState(float DeltaTime)
{
    if (!PlayerPawn) return;

    const FVector EnemyLoc = GetActorLocation();
    const FVector PlayerLoc = PlayerPawn->GetActorLocation();

    const float DistX = FMath::Abs(PlayerLoc.X - EnemyLoc.X);
    const float DetectX = DetectionRadius;
    const float AttackX = AttackRadius;

    switch (CurrentState)
    {
    case EEnemyState::Idle:
        if (DistX <= DetectX)
            SetState(EEnemyState::Chase);
        break;

    case EEnemyState::Chase:
        if (DistX > DetectX)
            SetState(EEnemyState::Idle);
        else if (DistX <= AttackX)
            SetState(EEnemyState::Attack);
        else
        {
            FacePlayer(PlayerLoc, EnemyLoc);
            MoveAlongX(EnemyLoc, PlayerLoc, DeltaTime);
        }
        break;

    case EEnemyState::Attack:
        if (DistX > AttackX)
            SetState(EEnemyState::Chase);
        else
        {
            FacePlayer(PlayerLoc, EnemyLoc);

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

void AEnemyCharacter::FacePlayer(const FVector& PlayerLoc, const FVector& EnemyLoc)
{
    const float DirectionX = PlayerLoc.X - EnemyLoc.X;
    SetActorRotation(FRotator(0.f, DirectionX > 0.f ? 0.f : 180.f, 0.f));
}

void AEnemyCharacter::MoveAlongX(const FVector& EnemyLoc, const FVector& PlayerLoc, float DeltaTime)
{
    const float DirectionX = (PlayerLoc.X - EnemyLoc.X) > 0.f ? 1.f : -1.f;

    FHitResult Hit;
    FVector TraceStart = EnemyLoc;
    FVector TraceEnd = EnemyLoc + FVector(DirectionX * 60.f, 0.f, 0.f);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    const bool bHitWall = GetWorld()->LineTraceSingleByChannel(
        Hit,
        TraceStart,
        TraceEnd,
        ECC_WorldStatic,
        Params
    );

    if (!bHitWall)
    {
        AddMovementInput(FVector(DirectionX, 0.f, 0.f));
    }
}

APawn* AEnemyCharacter::GetPlayerPawn() const
{
    return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyCharacter::OnEnterIdle()
{
    GetCharacterMovement()->StopMovementImmediately();
}

void AEnemyCharacter::OnEnterChase()
{
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
        UDamageType::StaticClass()
    );
}

void AEnemyCharacter::OnDied(AActor* DeadActor)
{
    GetCharacterMovement()->DisableMovement();
    SetActorEnableCollision(false);
    OnBossDied();
}