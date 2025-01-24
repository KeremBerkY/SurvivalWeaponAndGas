// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SurvivalEventTagAnimNotif.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API USurvivalEventTagAnimNotif : public UAnimNotify
{
	GENERATED_BODY()

	USurvivalEventTagAnimNotif();
	
public:

	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual FString GetNotifyName_Implementation() const override;
};
