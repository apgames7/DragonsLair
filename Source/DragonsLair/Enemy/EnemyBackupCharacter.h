// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "DragonsLair/Player/DragonCharacter.h"
#include "EnemyBackupCharacter.generated.h"

UCLASS()
class DRAGONSLAIR_API AEnemyBackupCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBackupCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Actions
	void Win();
	void Move();
	void Attack();
	void AttackEnd();
	
	//Move
	UPROPERTY(EditAnywhere, Category = "Move", BlueprintReadWrite)
	float speed = .5f;
	UPROPERTY(EditAnywhere, Category = "Move", BlueprintReadWrite)
	FVector vectorTowardsPlayer; //Vector from enemy to player
	UPROPERTY(EditAnywhere, Category = "Move", BlueprintReadWrite)
	float lengthBetweenEnemyPlayer = vectorTowardsPlayer.Length(); 
	UPROPERTY(EditAnywhere, Category = "Move", BlueprintReadWrite)
	FVector directionToPlayer = vectorTowardsPlayer.GetSafeNormal(0.0001); //The length between the enemy and the player.
	UPROPERTY(EditAnywhere, Category = "Move", BlueprintReadWrite)
	float attackLength = 350.f; //Distance between player and enemy before the enemy attacks.
	UPROPERTY(EditAnywhere, Category = "Move", BlueprintReadWrite)
	float chaseLength = 3000.f; //Distance between player and enemy before the enemy starts chasing. 
	
	//Combat
	UPROPERTY(EditAnywhere, Category = "Combat", BlueprintReadWrite)
	bool canAttack = false;
	UPROPERTY(EditAnywhere, Category = "Combat", BlueprintReadWrite)
	float lives = 10.f;
	UPROPERTY(EditAnywhere, Category = "Combat", BlueprintReadWrite)
	float maxLives = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat", BlueprintReadWrite)
	UBoxComponent* attackBox;
	UPROPERTY(EditAnywhere, Category = "Combat", BlueprintReadWrite)
	USphereComponent* hitBox;
	
	//Visuals
	UPROPERTY(EditAnywhere, Category = "Visuals", BlueprintReadWrite)
	UDecalComponent* shadow;
	UPROPERTY(EditAnywhere, Category = "Visuals", BlueprintReadWrite)
	UAnimationAsset* attackAnim;
	
	//Player
	UPROPERTY(EditAnywhere, Category = "Player", BlueprintReadWrite)
	ADragonCharacter* player;
	
	//Timer
	FTimerHandle timerHandle;
};
