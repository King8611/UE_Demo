// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/DecalComponent.h"
#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapComp = CreateDefaultSubobject<UBoxComponent>("OverlapComp");
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetupAttachment(RootComponent);
	DecalComp->DecalSize = FVector(200, 200, 200);
}

// Called when the game starts or when spawned
void AFPSExtractionZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn && MyPawn->bIsCarryingObjective) 
	{
		AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode) {
			GameMode->CompleteMission(MyPawn, true);
		}
	}
	else {
		UGameplayStatics::PlaySound2D(this, Sound);
	}
	UE_LOG(LogTemp, Log, TEXT("AFPSExtractionZone::BeginOverlap"));
}

// Called every frame
void AFPSExtractionZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

