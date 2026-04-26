// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle    UMETA(DisplayName = "Idle"),
	Chase   UMETA(DisplayName = "Chase"),
	Attack  UMETA(DisplayName = "Attack")
};

UCLASS()
class DRAGONSLAIR_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Detection

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Detection")
    float DetectionRadius = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Detection")
    float AttackRadius = 200.f;

    // Movement 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
    float RotationInterpSpeed = 8.f;

    // Combat 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
    float AttackDamage = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
    float AttackCooldown = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
    TSubclassOf<UDamageType> DamageTypeClass;
  
    // State

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
    EEnemyState CurrentState = EEnemyState::Idle;

    // Components

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UHealthComponent* HealthComponent;

protected:
    virtual void OnEnterIdle();
    virtual void OnEnterChase();
    virtual void OnEnterAttack();
    virtual void PerformAttack();

    UFUNCTION()
    virtual void OnDied(AActor* DeadActor);

private:
    void UpdateState(float DeltaTime);
    void SetState(EEnemyState NewState);
    void RotateTowardsPlayer(float DeltaTime, const FVector& PlayerLocation);
    APawn* GetPlayerPawn() const;

    UPROPERTY()
    APawn* PlayerPawn = nullptr;

    float AttackCooldownRemaining = 0.f;
};
