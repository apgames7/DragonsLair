#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class USphereComponent;
class ADragonCharacter;

UCLASS(Blueprintable)
class DRAGONSLAIR_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	AItemBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Components 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* Collision;

	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float FloatAmplitude = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float FloatSpeed = 1.f;

	FVector StartLocation;

	// Item behavior
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bDestroyOnPickup = true;

	// Blueprint-defined effect (implemented per item)
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void ApplyEffect(ADragonCharacter* Player);

	// Overlap handler
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