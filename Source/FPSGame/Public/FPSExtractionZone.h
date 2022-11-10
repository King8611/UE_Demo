// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundBase.h"
#include "FPSExtractionZone.generated.h"
class UBoxComponent;
UCLASS()
class FPSGAME_API AFPSExtractionZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSExtractionZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components");
	UBoxComponent* OverlapComp;

	UPROPERTY(VisibleAnywhere, Category = "Components");
	UDecalComponent* DecalComp;

	UPROPERTY(EditAnywhere, Category = "Sound");
	USoundBase* Sound;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};