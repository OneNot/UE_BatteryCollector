// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyProjectTestGameMode.h"
#include "MyProjectTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AMyProjectTestGameMode::AMyProjectTestGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PowerDecayRate = 0.01f;
}

void AMyProjectTestGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMyProjectTestCharacter* MyChar = Cast<AMyProjectTestCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyChar)
	{
		if (MyChar->GetCurrentPower() > PowerToWin)
			SetCurrentState(EBatteryPlayState::EWon);
		else if (MyChar->GetCurrentPower() > 0)
			MyChar->UpdatePower(-DeltaTime * PowerDecayRate * MyChar->GetStartingPower());
		else
			SetCurrentState(EBatteryPlayState::EGameOver);
	}
}

float AMyProjectTestGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

void AMyProjectTestGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

	AMyProjectTestCharacter* MyCharacter = Cast<AMyProjectTestCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		PowerToWin = (MyCharacter->GetStartingPower()) * 1.25f;
	}

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

EBatteryPlayState AMyProjectTestGameMode::GetCurrentPlayState() const
{
	return CurrentState;
}

void AMyProjectTestGameMode::SetCurrentState(EBatteryPlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void AMyProjectTestGameMode::HandleNewState(EBatteryPlayState NewState)
{
	switch (NewState)
	{
		case EBatteryPlayState::EPlaying:
		{
			for (ASpawnVolume* Volume : SpawnVolumeActors)
				Volume->SetSpawningActive(true);
			break;
		}
		case EBatteryPlayState::EWon:
		{
			for (ASpawnVolume* Volume : SpawnVolumeActors)
				Volume->SetSpawningActive(false);
			break;
		}
		case EBatteryPlayState::EGameOver:
		{
			for (ASpawnVolume* Volume : SpawnVolumeActors)
				Volume->SetSpawningActive(false);

			//block inputs
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController)
				PlayerController->SetCinematicMode(true, false, false, true, true);

			//ragdoll
			ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
			if (MyCharacter)
			{
				MyCharacter->GetMesh()->SetSimulatePhysics(true);
				MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			}
			break;
		}
		case EBatteryPlayState::EUnknown:
		{
			break;
		}
	}
}
