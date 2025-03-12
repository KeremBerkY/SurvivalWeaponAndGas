// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalPlayerController.h"

ASurvivalPlayerController::ASurvivalPlayerController()
{
	HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId ASurvivalPlayerController::GetGenericTeamId() const
{
	return HeroTeamID;
}
