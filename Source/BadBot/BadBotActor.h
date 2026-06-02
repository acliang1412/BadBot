// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BadBotActor.generated.h"

UCLASS()
class BADBOT_API ABadBotActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABadBotActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Exposes a speed variable to your Unreal Engine details panel!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BadBot Mechanics")
	float RotationSpeed;

};
