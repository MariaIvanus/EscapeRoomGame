// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"


#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE1_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//How far Player can reach with his hands in cm
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputHandle = nullptr;
	
	//Ray cast and grab what in reach
	void Grab();

	//Put object that player grab
	void Release();

	//Find Physics Component attached to object
	void FindPhysicsHandleComponent();

	//Set InputComponent attaced to object in runtine (expect this)
	void SetInputComponent();

	//Return hit for first physics body in reach 
	const FHitResult GetFirstPhysicsBodyInReach();


	//Get the origin player posityion (and start of the ray cast where player look)
	FVector GetLineTraceStart();

	//Get the end of the ray cast where player look
	FVector GetLineTraceEnd();
	
};
