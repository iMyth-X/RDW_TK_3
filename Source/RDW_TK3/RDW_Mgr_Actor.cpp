// Fill out your copyright notice in the Description page of Project Settings.

#include "RDW_Mgr_Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"


// Sets default values
ARDW_Mgr_Actor::ARDW_Mgr_Actor() :
	MaxTransGain(0.26f),
	MinTransGain(-.14f),
	MinRotGain(.49),
	MaxRotGain(-.2f),
	CurvatureRadius(7.5f),
	TrackedSpacePosition(0.0, 0.0, 0.0),
	TrackedSpaceRotation(0.0, 0.0, 0.0),
	TrackedSpaceScale(10.0, 10.0, 1.0)		// Hardcoding the scale for now, embarrasing I know!
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	startTimeOfProgram = FDateTime::Now();
	//UE_LOG(LogTemp, Warning, TEXT("STart time of program = %s"), *startTimeOfProgram.ToString());

	constructRedirectedUser();
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

// Called whenever a property  value changes
//void ARDW_Mgr_Actor::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
//{
//	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
//
//	if ((PropertyName == GET_MEMBER_NAME_CHECKED(UPaperTileLayer, LayerWidth)) || (PropertyName == GET_MEMBER_NAME_CHECKED(UPaperTileLayer, LayerHeight)))
//		Do some stuff
//	}
//}

// Build all of the procedural elements that make up the Redirected User
// I only want there to be one object added to the scene
void ARDW_Mgr_Actor::constructRedirectedUser()
{
	// Create a scene component which will act as the parent for everything
	topNode = CreateDefaultSubobject<USceneComponent>(TEXT("Redirected User"));
	RootComponent = topNode;

	constructAvatar();
	constructTrackedSpace();
	constructSimulatedUser();
}

// Procedural creation of Avatar Body. I supposed this could have been created with a Blueprint.
// However I am looking to create this procedurally so the code will be more less locked and less
// easily changed.
void ARDW_Mgr_Actor::constructAvatar()
{
	// The scene compnent will act as the container for the avatar
	body = CreateDefaultSubobject<USceneComponent>(TEXT("Body"));
	body->SetupAttachment(topNode);
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

// Creating another procedural element in the Redireection Manager/
// This one stablished the tracked area foot print and the basic bounding/trigger box
void ARDW_Mgr_Actor::constructTrackedSpace()
{
	float planeHeightScale = 0.1;
	float resetColliderHeight = 50;
	// The scene compnent will act as the container for the aTracked space
	trackedSpace = CreateDefaultSubobject<USceneComponent>(TEXT("Tracked Space"));
	trackedSpace->SetupAttachment(topNode);

	//Create the box which will act as the floor plane for the tracked space
	UStaticMeshComponent* plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	plane->SetupAttachment(trackedSpace);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>planeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (planeAsset.Succeeded())
	{
		plane->SetStaticMesh(planeAsset.Object);
		plane->SetRelativeScale3D(FVector(TrackedSpaceScale.X, TrackedSpaceScale.Y, planeHeightScale));
		FTransform planeTrans = FTransform(FRotator(0.0, 0.0, 0.0), FVector(1.0, 1.0, 1.0));
		FVector planeBox = plane->CalcBounds(planeTrans).BoxExtent;
		//UE_LOG(LogTemp, Warning, TEXT("planeBox=%s"), *planeBox.ToString());
		//This statement does not work properly, if it did I would not need to multiply by planeScaleHeight
		plane->SetRelativeLocation(FVector(0.0, 0.0, -planeHeightScale*planeBox.Z));
	}

	//Create a reset collider which is just a box about the size of the tracked SPace
	UBoxComponent* resetCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ResetCollider"));
	resetCollider->SetupAttachment(trackedSpace);
	FVector min, max;
	plane->GetLocalBounds(min, max);
	resetCollider->SetBoxExtent(FVector(50.0*TrackedSpaceScale.X, 50.0*TrackedSpaceScale.Y, resetColliderHeight));
	// The plane's local bounds aren't quite working, else I would not need to multiply by planeScaleHeight
	resetCollider->SetRelativeLocation(FVector(0.0, 0.0, resetCollider->GetUnscaledBoxExtent().Z-planeHeightScale*(max.Z-min.Z)));

	//Apply the tracked space modifications. In the future the size and scale of this platform will be set by the UPROPERTY
	trackedSpace->SetRelativeLocationAndRotation(TrackedSpacePosition, TrackedSpaceRotation);
}

// Creating another procedural element in the Redireection Manager/
// This one Houses the simulation Camera that will display the simulation POV
void ARDW_Mgr_Actor::constructSimulatedUser()
{
	// The scene compnent will act as the container for the simulated User
	simulatedUser = CreateDefaultSubobject<USceneComponent>(TEXT("Simulated User"));
	simulatedUser->SetupAttachment(topNode);

	//Create the camera which will act as the view spot for the simulated User
	simulatedHead = CreateDefaultSubobject<UCameraComponent>(TEXT("Simulated Head"));
	simulatedHead->SetupAttachment(simulatedUser);
	// I'm not real happy with the location of the camera. Hopefully this won't impact performance
	// But maybe I am just setting myself up for trouble
	simulatedHead->SetRelativeLocationAndRotation(FVector(0.0, 20.0, 62.0), FRotator(0.0, 90.0, 0.0));
}