// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnHealthChanged,
	UHealthComponent*, HealthComp,
	float, Health,
	float, HealthDelta,
	const UDamageType*, DamageType
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGONSLAIR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    bool bIsDead = false;

    UPROPERTY(BlueprintAssignable, Category = "Health|Events")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Health|Events")
    FOnDeath OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float Amount);

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercent() const { return (MaxHealth > 0.f) ? CurrentHealth / MaxHealth : 0.f; }

private:
    UFUNCTION()
    void HandleTakeAnyDamage(
        AActor* DamagedActor,
        float              Damage,
        const UDamageType* DamageType,
        AController* InstigatedBy,
        AActor* DamageCauser
    );
};