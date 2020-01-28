// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyProjectTestGameMode.h"
#include "MyProjectTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

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
		if(MyChar->GetCurrentPower() > 0)
			MyChar->UpdatePower(-DeltaTime * PowerDecayRate * MyChar->GetStartingPower());
	}
}
