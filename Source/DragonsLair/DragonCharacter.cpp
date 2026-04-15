// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ADragonCharacter::ADragonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetRelativeRotation(FRotator{0.f, -90.f, 0.f});
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	WeaponMesh->SetupAttachment(GetMesh());
	WeaponMesh->SetRelativeRotation(FRotator{0.f, -90.f, 0.f});

}

// Called when the game starts or when spawned
void ADragonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
		
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

}

// Called to bind functionality to input
void ADragonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADragonCharacter::PlayerMove);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADragonCharacter::PlayerJump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADragonCharacter::PlayerAttack);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ADragonCharacter::PlayerDash);
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
	
	if (!ADragonCharacter::inBossFight) {
		
		float vLength = sqrt(ActionVector.Y * ActionVector.Y + ActionVector.X * ActionVector.X);
		FVector3d worldDirection = 
		{ActionVector.Y / vLength, ActionVector.X / vLength, 0.f};
		AddMovementInput(worldDirection, 1);
	}
	
	
		
}

void ADragonCharacter::PlayerJump() {
	Jump();
}

void ADragonCharacter::PlayerAttack() {
	
}

void ADragonCharacter::PlayerDash() {
	
}
