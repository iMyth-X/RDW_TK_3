// Fill out your copyright notice in the Description page of Project Settings.

#include "RDW_Mgr_Actor.h"
#include "Components/CapsuleComponent.h"
#include "ConstructorHelpers.h"


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

	constructAvatar();
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
	// The scene compnent will act as the container for the avatar
	body = CreateDefaultSubobject<USceneComponent>(TEXT("Body"));
	RootComponent = body;
	body->SetRelativeLocation(FVector(0.0, 0.0, 44.0));
	
	//Add the components of the Body Avatar
	// The first thing to add is the collision capsule
	UCapsuleComponent* CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	CapsuleComponent->SetupAttachment(body);
	CapsuleComponent->SetCapsuleHalfHeight(44.0);
	CapsuleComponent->SetCapsuleRadius(22.0);
	CapsuleComponent->SetRelativeScale3D(FVector(0.75, 0.75, 1.0));

	// Create a visual cylinder
	UStaticMeshComponent* visualBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Representation"));
	visualBody->SetupAttachment(body);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> visualBodyAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (visualBodyAsset.Succeeded())
	{
		visualBody->SetStaticMesh(visualBodyAsset.Object);
		visualBody->SetWorldScale3D(FVector(0.3, 0.3, 0.85));
		visualBody->SetMaterial(0, Material);
	}

	//Create A cyclinder which will act as the orientation
	UStaticMeshComponent* visualOrientation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orientation"));
	visualOrientation->SetupAttachment(body);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> visualOrientationAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (visualOrientationAsset.Succeeded())
	{
		visualOrientation->SetStaticMesh(visualBodyAsset.Object);
		visualOrientation->SetWorldScale3D(FVector(0.075, 0.075, 0.1));
		visualOrientation->SetRelativeLocationAndRotation(FVector(0.0, 20.0, 20.0), FRotator(90.0, 0.0, 90.0));
		visualOrientation->SetMaterial(0, Material);
	}
}

