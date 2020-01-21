// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"
#include "Components/StaticMeshComponent.h"

ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);

	BatteryPower = 150.0f;
}

void ABatteryPickup::OnCollected_Implementation()
{
	Super::OnCollected_Implementation();
	Destroy();
}

float ABatteryPickup::GetPower()
{
	return BatteryPower;
}
