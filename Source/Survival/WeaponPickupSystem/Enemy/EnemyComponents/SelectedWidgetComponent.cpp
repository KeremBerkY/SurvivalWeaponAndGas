// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedWidgetComponent.h"


USelectedWidgetComponent::USelectedWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetVisibility(false);

}

void USelectedWidgetComponent::ShowSelectedWidget()
{
	SetVisibility(true);
}

void USelectedWidgetComponent::HideSelectedWidget()
{
	SetVisibility(false);
}


void USelectedWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USelectedWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

