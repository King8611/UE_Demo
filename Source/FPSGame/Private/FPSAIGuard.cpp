// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
	GuardState = EAIState::Idle;
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::OnPawnSeen(APawn* Pawn)
{
	if (!Pawn)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32, 12, FColor::Blue, false, 10);

	SetGuardState(EAIState::Alerted);
	AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->CompleteMission(Pawn, false);
	}
}

// Called to bind functionality to input
void AFPSAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), NoiseInstigator->GetActorLocation(), 32, 12, FColor::Yellow, false, 10);
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandleResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandleResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3);

	SetGuardState(EAIState::Suspicious);
}


void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (NewState == GuardState)
	{
		return;
	}
	GuardState = NewState;
	OnChangeGuardState(GuardState);
}