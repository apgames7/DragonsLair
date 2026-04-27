#include "HealthComponent.h"
#include "GameFramework/Actor.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
    bIsDead = false;

    if (AActor* Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
    }
}

void UHealthComponent::HandleTakeAnyDamage(
    AActor* DamagedActor,
    float              Damage,
    const UDamageType* DamageType,
    AController* InstigatedBy,
    AActor* DamageCauser)
{
    if (Damage <= 0.f || bIsDead) return;

    const float PrevHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
    const float Delta = CurrentHealth - PrevHealth;

    OnHealthChanged.Broadcast(this, CurrentHealth, Delta, DamageType);

    if (CurrentHealth <= 0.f)
    {
        bIsDead = true;
        OnDeath.Broadcast(GetOwner());
    }
}

void UHealthComponent::Heal(float Amount)
{
    if (Amount <= 0.f || bIsDead) return;

    const float PrevHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
    const float Delta = CurrentHealth - PrevHealth;

    OnHealthChanged.Broadcast(this, CurrentHealth, Delta, nullptr);
}