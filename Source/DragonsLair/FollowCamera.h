// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FollowCamera.generated.h"

class UCameraComponent;
class UBoxComponent; 
class USplineComponent;

UCLASS()
class DRAGONSLAIR_API AFollowCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFollowCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//virtual void OnConstruction(const FTransform& Transform) override;
	
	//Camera
	UPROPERTY(EditAnywhere, Category = "Camera", BlueprintReadWrite);
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, Category = "Camera", BlueprintReadWrite);
	FVector cameraRightVector;
	UPROPERTY(EditAnywhere, Category = "Camera", BlueprintReadWrite);
	FVector cameraForwardVector;
	
	//Triggers
	UPROPERTY(EditAnywhere, Category = "Triggers", BlueprintReadWrite);
	UBoxComponent* BossRoom;
	
	UPROPERTY(EditAnywhere, Category = "Triggers", BlueprintReadWrite);
	UBoxComponent* StopPlayerFromGoingBack;	
	
	UPROPERTY(EditAnywhere, Category = "Triggers", BlueprintReadWrite);
	UBoxComponent* RotateTorwardsPlayer;
	
	//Spline
	UPROPERTY(EditAnywhere, Category = "Spline", BlueprintReadWrite);
	USplineComponent* CameraPath;
};
