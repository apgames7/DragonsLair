#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class USphereComponent;
class ABP_DragonPlayer;

UCLASS(Blueprintable)
class DRAGONSLAIR_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	AItemBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	USphereComponent* Collision;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float FloatAmplitude = 20.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float FloatSpeed = 1.f;

	FVector StartLocation;

	// Blueprint will implement this
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void ApplyEffect(ABP_DragonPlayer* Player);

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};