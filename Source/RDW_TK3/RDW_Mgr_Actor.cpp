// Fill out your copyright notice in the Description page of Project Settings.

#include "RDW_Mgr_Actor.h"


// Sets default values
ARDW_Mgr_Actor::ARDW_Mgr_Actor():
	MaxTransGain(0.26f),
	MinTransGain(-.14f),
	MinRotGain(.49),
	MaxRotGain(-.2f),
	CurvatureRadius(7.5f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	startTimeOfProgram = FDateTime::Now();
	//UE_LOG(LogTemp, Warning, TEXT("STart time of program = %s"), *startTimeOfProgram.ToString());
}

// Called when the game starts or when spawned
void ARDW_Mgr_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARDW_Mgr_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARDW_Mgr_Actor::constructAvatar()
{

}

void ARDW_Mgr_Actor::getBody()
{

}

