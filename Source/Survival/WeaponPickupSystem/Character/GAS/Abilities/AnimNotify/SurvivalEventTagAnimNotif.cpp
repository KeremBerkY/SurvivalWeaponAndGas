// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalEventTagAnimNotif.h"

#include "AbilitySystemBlueprintLibrary.h"

USurvivalEventTagAnimNotif::USurvivalEventTagAnimNotif()
{
#if WITH_EDITORONLY_DATA	
	bShouldFireInEditor = false;
#endif
}

void USurvivalEventTagAnimNotif::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (const auto Actor = MeshComp->GetOwner())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, FGameplayEventData());
	}
}

FString USurvivalEventTagAnimNotif::GetNotifyName_Implementation() const
{
	return EventTag.GetTagName().ToString();

}
