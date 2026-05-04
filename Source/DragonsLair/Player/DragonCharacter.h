// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Components/BoxComponent.h"
#include "DragonCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class DRAGONSLAIR_API ADragonCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADragonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	//Actions	
	void PlayerMove(const FInputActionValue& ActionValue);
	void PlayerJump();
	void PlayerDash();
	void PlayerDashEnd();
	void PlayerDashStarted();
	void PlayerDashCancelled();
	void PlayerDashCompleted();
	
	void PlayerAttack();
	void PlayerAttackEnd();
	void PlayerDie(); //Restarts level under certain conditons.
	
	//Controller
	UPROPERTY(EditAnywhere, Category = "Input");
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input");
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input");
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input");
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input");
	UInputAction* DashAction;
	
	//Movement
	UPROPERTY(EditAnywhere, Category = "Player", BlueprintReadWrite)	
	bool canDash = true;
	UPROPERTY(EditAnywhere, Category = "Player", BlueprintReadWrite)	
	bool isDashing = false;

	//Combat
	UPROPERTY(EditAnywhere, Category = "Player", BlueprintReadWrite)
	bool inBossFight;
	UPROPERTY(EditAnywhere, Category = "Player", BlueprintReadWrite)
	bool canHaveSword = false;
	UPROPERTY(EditAnywhere, Category = "Player", BlueprintReadWrite)
	bool isAttacking;
	UPROPERTY(EditAnywhere, Category = "Player", BlueprintReadWrite)
	float lives = 1.f;
	UPROPERTY(EditAnywhere, Category = "Player", BlueprintReadWrite)
	float maxLives = 10.f;
	UPROPERTY(EditAnywhere, Category = "Player", BlueprintReadWrite)
	UBoxComponent* hitBox;
	
	//Camera
	UPROPERTY(EditAnywhere, Category = "Camera", BlueprintReadWrite)
	AActor* FollowCamera;
	
	//Weapon
	UPROPERTY(EditAnywhere, Category = "Weapons")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName HandleSocket;
	
	FTimerHandle timerHandle;
};
