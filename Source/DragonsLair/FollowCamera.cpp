// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowCamera.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
AFollowCamera::AFollowCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	BossRoom = CreateDefaultSubobject<UBoxComponent>("BossRoom");
	StopPlayerFromGoingBack = CreateDefaultSubobject<UBoxComponent>("StopPlayerFromGoingBack");
	RotateTorwardsPlayer = CreateDefaultSubobject<UBoxComponent>("RotateTowardsPlayer");
	CameraPath = CreateDefaultSubobject<USplineComponent>("CameraPath");

}

// Called when the game starts or when spawned
void AFollowCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFollowCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//BossRoom->OverlapComponent()
}
/*
void AFollowCamera::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	
	CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
}
*/


