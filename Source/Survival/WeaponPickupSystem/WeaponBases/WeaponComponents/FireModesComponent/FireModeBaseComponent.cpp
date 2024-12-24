// Fill out your copyright notice in the Description page of Project Settings.


#include "FireModeBaseComponent.h"


UFireModeBaseComponent::UFireModeBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UFireModeBaseComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UFireModeBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UFireModeBaseComponent::Fire()
{
	IFire::Fire();
}

void UFireModeBaseComponent::EndFire()
{
	IFire::EndFire();
}

