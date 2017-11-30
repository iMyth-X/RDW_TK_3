// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RDW_Mgr_Actor.generated.h"

// Still not correct, I need to figure how to embed this within the class itself
UENUM(BlueprintType)
enum EMovementController { keyboard, AutoPilot, Tracker };

UCLASS()
class RDW_TK3_API ARDW_Mgr_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARDW_Mgr_Actor();

	//AddToolTip: "Select if you wish to run simulation from commandline in UE4 batchmode."
	//Needed?
	UPROPERTY(EditAnywhere, Category = "Redirected Walking")
		bool runInTestMode = false;

	//AddToolTip: "How user movement is controlled"
	/****************   Gotto Fix!!!! *****************************/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Redirected Walking")
	//	EMovementController MOVEMENT_CONTROLLER = Tracker;

	//ToolTip: "Maximum translation gain applied"
	//Range: (0, 5)
	UPROPERTY(EditAnywhere, Category = "Redirected Walking")
		float MaxTransGain;

	//ToolTip: "Minimumlation gain applied"
	//Range: (-.99f, 0)
	UPROPERTY(EditAnywhere, Category = "Redirected Walking")
		float MinTransGain;

	//ToolTip: "Maximum rotation gain applied"
	//Range: (0, 5)
	UPROPERTY(EditAnywhere, Category = "Redirected Walking")
		float MaxRotGain;

	//ToolTip: "Minimumlation rotation gain applied"
	//Range: (-.99f, 0)
	UPROPERTY(EditAnywhere, Category = "Redirected Walking")
		float MinRotGain;

	//ToolTip: "Radius applied by curvature gain"
	//Range: (1, 23)
	UPROPERTY(EditAnywhere, Category = "Redirected Walking")
		float CurvatureRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
