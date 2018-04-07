// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT //for paramentrs, that are changed by a function by reference

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Reported for duties"));
	FindPhysicsHandleComponent();
	SetInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	//if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		//move the object we holding
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

//Find Physics Component attached to object
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing UPhysicsHandleComponent"), *GetOwner()->GetName());
	}
}

//Look for and setup attached InputHandle component(only appears in Runtime)
void UGrabber::SetInputComponent() {
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputHandle) {
		InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputHandle->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing UInputComponent(PawnInput)"), *GetOwner()->GetName());
	}
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab is pressed"));

	//Line trace (Ray cast) and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	//no bones needed
	//If we hit something then 
	//attach a physics handle
	if (ActorHit) {
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, 
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}


}

// Relese physics handle
void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Release is pressed"));
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}


FVector UGrabber::GetLineTraceStart() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);
	return PlayerViewPointLocation;
}

// Get the end of where player look
FVector UGrabber::GetLineTraceEnd() {
	// Get the player view point every tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	// Get the end of where player look
	return PlayerViewPointLocation + (PlayerViewPointRotator.Vector() * Reach);

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult LineTraceHit;

	//Setup query parametrs
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	//Raycasting out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		GetLineTraceStart(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return LineTraceHit;
}


#pragma region OldTick
//// Get the player view point every tick
//FVector PlayerViewPointLocation;
//FRotator PlayerViewPointRotator;
//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
//	OUT PlayerViewPointLocation,
//	OUT PlayerViewPointRotator
//);
//
/////TODO: Test value
/////UE_LOG(LogTemp, Warning, TEXT("Players location: %s Player rotation: %s"), 
/////	*PlayerViewPointLocation.ToString(), 
/////	*PlayerViewPointRotator.ToString());
//
////Draw red trace in the world to visualize
//FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotator.Vector() * Reach);
//DrawDebugLine(
//	GetWorld(),
//	PlayerViewPointLocation,
//	LineTraceEnd,
//	FColor(255, 0, 0),
//	false,
//	0.0f,
//	0.0f,
//	10.0f
//);
//
////Setup query parametrs
//FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
//
////Raycasting out to reach distance
//FHitResult LineTraceHit;
//GetWorld()->LineTraceSingleByObjectType(
//	OUT LineTraceHit,
//	PlayerViewPointLocation,
//	LineTraceEnd,
//	FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
//	TraceParameters
//);
//
////Log what we hit
//AActor * ActorHit = LineTraceHit.GetActor();
//if (ActorHit) {
//	UE_LOG(LogTemp, Warning, TEXT("Hit: %S"), *ActorHit->GetName());
//}
////See what we hit

#pragma endregion


