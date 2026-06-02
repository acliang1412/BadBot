// Fill out your copyright notice in the Description page of Project Settings.


#include "BadBotActor.h"

// Sets default values
ABadBotActor::ABadBotActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set a default rotation speed starting value
	RotationSpeed = 100.0f;

}

// Called when the game starts or when spawned
void ABadBotActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABadBotActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Create a new rotation calculation based on our speed and frame rate
	FRotator NewRotation = FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f);

	// Apply this rotation to the actor in the world
	AddActorLocalRotation(NewRotation);
}

