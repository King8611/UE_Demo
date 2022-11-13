// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}
void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bIsSuccess)
{
	if (InstigatorPawn) {
		UE_LOG(LogTemp, Log, TEXT("AFPSGameMode::CompleteMission !!"));
		InstigatorPawn->DisableInput(nullptr);
		OnMissionComplete(InstigatorPawn, bIsSuccess);
	}

	if (!SpectatingViewpointClass) {
		return;
	}
	APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
	if (!PC) {
		return;
	}
	TArray<AActor*> ReturnedActors;
	UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);
	if (!ReturnedActors.Num()) 
	{
		return;
	}
	PC->SetViewTargetWithBlend(ReturnedActors[0], 0.5, EViewTargetBlendFunction::VTBlend_Cubic);
}