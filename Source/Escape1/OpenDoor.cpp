// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Grabber.h"
#include "Components/PrimitiveComponent.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//Find the owning Actor
	Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner of OpenDoor.cpp is missing"));
		return;
	}

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressurePlate"),*GetOwner()->GetName());
	}
}
// Called every frame

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the triggervolue(PressurePlate) if the ActorThatOpens in it
	//if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) //TODO make a parameter;
	{
		OnOpen.Broadcast();
	} else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	if (!PressurePlate) { return TotalMass; }
	
	//Find all overlapping actors
	TArray<AActor*> OverlapingActors;

	PressurePlate->GetOverlappingActors(
		OUT OverlapingActors
	);
	//Iterate through them adding their masses
	for (const auto* Actor : OverlapingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on plate"), *Actor->GetName());

	}

	return TotalMass;
}
