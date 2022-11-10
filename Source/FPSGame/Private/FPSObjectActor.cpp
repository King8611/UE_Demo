// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AFPSObjectActor::AFPSObjectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SphereComp->SetupAttachment(MeshComp);
}

void AFPSObjectActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

// Called when the game starts or when spawned
void AFPSObjectActor::BeginPlay()
{
	Super::BeginPlay();
	PlayEffects();
}


// Called every frame
void AFPSObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSObjectActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	PlayEffects();
	AFPSCharacter* Actor = Cast<AFPSCharacter>(OtherActor);
	if (Actor) {
		Actor->bIsCarryingObjective = true;
	}
	Destroy();
}