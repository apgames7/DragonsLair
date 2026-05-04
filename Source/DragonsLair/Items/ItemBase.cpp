#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "BP_DragonPlayer.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Collision->InitSphereRadius(100.f);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
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

	float Time = GetGameTimeSinceCreation() * FloatSpeed;
	float OffsetZ = FMath::Sin(Time) * FloatAmplitude;

	FVector NewLocation = StartLocation;
	NewLocation.Z += OffsetZ;

	SetActorLocation(NewLocation);
}

void AItemBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	ABP_DragonPlayer* Player = Cast<ABP_DragonPlayer>(OtherActor);
	if (!Player) return;

	// Call Blueprint logic
	ApplyEffect(Player);

	Destroy();
}