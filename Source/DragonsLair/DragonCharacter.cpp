// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FollowCamera.h"
#include "Rendering/RenderCommandPipes.h"

// Sets default values
ADragonCharacter::ADragonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetRelativeRotation(FRotator{0.f, -90.f, 0.f});
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	WeaponMesh->SetRelativeRotation(FRotator{0.f, -90.f, 0.f});
	WeaponMesh->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void ADragonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	
		
	FollowCamera = UGameplayStatics::GetActorOfClass(GetWorld(), AFollowCamera::StaticClass());
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) {
			
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)) {
				
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
	};
	
}

// Called every frame
void ADragonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Set the weapon visible only after you have picked it up.
	if (canHaveSword) WeaponMesh->SetHiddenInGame(false); 
	else WeaponMesh->SetHiddenInGame(true);
	
	//Kill player
	PlayerDie();

}

// Called to bind functionality to input
void ADragonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADragonCharacter::PlayerMove);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADragonCharacter::PlayerJump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADragonCharacter::PlayerAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ADragonCharacter::PlayerAttackEnd);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ADragonCharacter::PlayerAttackEnd);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ADragonCharacter::PlayerDash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ADragonCharacter::PlayerDashStarted);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Canceled, this, &ADragonCharacter::PlayerDashCancelled);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ADragonCharacter::PlayerDashCompleted);
	}
}

void ADragonCharacter::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	
	WeaponMesh->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		HandleSocket
	);
}

//Moving player based with a normalized vector.
void ADragonCharacter::PlayerMove(const FInputActionValue& ActionValue) {
	
	FVector2D ActionVector = ActionValue.Get<FVector2D>();

	AddMovementInput(FollowCamera->GetActorForwardVector(), ActionVector.Y);
	AddMovementInput(FollowCamera->GetActorRightVector(), ActionVector.X);
	
}

void ADragonCharacter::PlayerJump() {
	Jump();
}

void ADragonCharacter::PlayerAttack() {
	
	if (canHaveSword) {
		isAttacking = true;		
	}
	
}

void ADragonCharacter::PlayerAttackEnd() {
	
	isAttacking = false;
}

//Player Dash
void ADragonCharacter::PlayerDash() {
	
	if (canDash) {
		canDash = false;
		GetCharacterMovement()->GroundFriction = 0.f;
		GetCharacterMovement()->AddImpulse(GetMesh()->GetForwardVector().RotateAngleAxis(90.f, {0, 0, 1}) * FVector3d{1500.f, 1500.f, 1500.f}, true);
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ThisClass::PlayerDashEnd, 3.f, false);
	}	
}

void ADragonCharacter::PlayerDashEnd() {
	isDashing = false;
	canDash = true;
	GetCharacterMovement()->GroundFriction = 8.f;
}

void ADragonCharacter::PlayerDashStarted() {
	if (canDash) isDashing = true;
}

void ADragonCharacter::PlayerDashCancelled() {
	GetCharacterMovement()->GroundFriction = 8.f;	
	isDashing = false;
}

void ADragonCharacter::PlayerDashCompleted() {
	GetCharacterMovement()->GroundFriction = 8.f;	
	isDashing = false;
}

void ADragonCharacter::PlayerDie() {
	
	if (GetActorLocation().Z > -20.f) return;
	if (lives > 0.f) return;
	//UGameplayStatics::OpenLevel(GetWorld(), FName("DeathMenu"));
	UGameplayStatics::LoadStreamLevel(this, FName("DeathMenu"), true, true, FLatentActionInfo());
}

