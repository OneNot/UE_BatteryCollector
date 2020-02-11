// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectTestGameMode.generated.h"

UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class AMyProjectTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyProjectTestGameMode();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentPlayState() const;

	void SetCurrentState(EBatteryPlayState NewState);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float PowerDecayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float PowerToWin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	EBatteryPlayState CurrentState;
	TArray<class ASpawnVolume*> SpawnVolumeActors;
	void HandleNewState(EBatteryPlayState NewState);
};



