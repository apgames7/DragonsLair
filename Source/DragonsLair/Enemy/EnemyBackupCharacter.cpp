// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBackupCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "DragonsLair/Player/DragonCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBackupCharacter::AEnemyBackupCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	attackBox = CreateDefaultSubobject<UBoxComponent>("Attack Box");
	hitBox = CreateDefaultSubobject<USphereComponent>("Hitbox");
	
	shadow = CreateDefaultSubobject<UDecalComponent>("Shadow");
}

// Called when the game starts or when spawned
void AEnemyBackupCharacter::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<ADragonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void AEnemyBackupCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Win();
	Move();
	Attack();
	
}

// Called to bind functionality to input
void AEnemyBackupCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBackupCharacter::Win() {
	
	if (ActorHasTag(FName("Boss")) && lives <= 0) UGameplayStatics::OpenLevel(GetWorld(), FName("WinMenu"), true);
}

void AEnemyBackupCharacter::Move() {

	if (!player) return;
	//Update position variables
	vectorTowardsPlayer = player->GetMesh()->GetComponentLocation() - GetActorLocation();
	lengthBetweenEnemyPlayer = vectorTowardsPlayer.Length();
	directionToPlayer = vectorTowardsPlayer.GetSafeNormal(0.0001);
	
	if (lengthBetweenEnemyPlayer < chaseLength &&
		lengthBetweenEnemyPlayer > attackLength) {
		AddMovementInput(directionToPlayer, speed, true);
	}
}

void AEnemyBackupCharacter::Attack() {

	if (!player) return;
	if (lengthBetweenEnemyPlayer < attackLength) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, ("within range"));
	if (canAttack) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, TEXT("can attack"));
	if (attackBox->IsOverlappingComponent(player->GetCapsuleComponent())) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Silver, TEXT("overlapping"));
	if (lengthBetweenEnemyPlayer < attackLength && 
		canAttack && 
		attackBox->IsOverlappingComponent(player->GetCapsuleComponent())
		) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("Attack"));
		
		canAttack = false;
		player->lives -= 1;
		GetMesh()->PlayAnimation(attackAnim, false);
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ThisClass::AttackEnd, 3.f, false);
	}
		
}

void AEnemyBackupCharacter::AttackEnd() {
	canAttack = true;	
}

