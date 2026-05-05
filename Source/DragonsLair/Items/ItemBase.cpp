#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "DragonsLair/Player/DragonCharacter.h"
#include "Engine/Engine.h"

AItemBase::AItemBase()
{
    PrimaryActorTick.bCanEverTick = true;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SetRootComponent(Collision);

    Collision->InitSphereRadius(100.f);

    // Explicit collision setup
    Collision->SetCollisionObjectType(ECC_WorldDynamic); // Default for pick-up items
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // Overlap only Pawns
    Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
    Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    Collision->SetGenerateOverlapEvents(true);
}

void AItemBase::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();

    Collision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
}

void AItemBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const float Time = GetGameTimeSinceCreation() * FloatSpeed;
    const float OffsetZ = FMath::Sin(Time) * FloatAmplitude;

    FVector NewLocation = StartLocation;
    NewLocation.Z += OffsetZ;

    SetActorLocation(NewLocation); // Optionally add 'true' for Sweep!
}

void AItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
#if !UE_BUILD_SHIPPING
    if (!OtherActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("AItemBase overlap: OtherActor null"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("AItemBase overlap: OtherActor is %s (%s)"), *OtherActor->GetName(), *OtherActor->GetClass()->GetName());
#endif

    ADragonCharacter* Player = Cast<ADragonCharacter>(OtherActor);
    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("AItemBase: Overlap actor is NOT a DragonCharacter."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("AItemBase: Overlap actor IS a DragonCharacter. Calling ApplyEffect."));
    ApplyEffect(Player);

    if (bDestroyOnPickup)
    {
        Destroy();
    }
}