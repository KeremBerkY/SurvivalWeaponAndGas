// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UPickupComponent;
class UProgressBar;
class UTextBlock;
struct FInteractableData;
class ASurvivalCharacter;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	ASurvivalCharacter* PlayerReference;

	UFUNCTION()
	void UpdateWidget(const FInteractableData& InteractableData);
	UFUNCTION()
	void ShowWidget();
	UFUNCTION()
	void HideWidget();
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* NameText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* ActionText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* QuantityText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* KeyPressText; // exp: hold instead of press
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UProgressBar* InteractionProgressBar; // Used for time interactables

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	float CurrentInteractionDuration;

	UFUNCTION(Category = "Interaction Widget | Interactable Data")
	float UpdateInteractionProgress(); // For ProgressBar. returns a float and this is how the progress bar knows how much to fill up.

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	void BindShowAndHideCallbacks(UPickupComponent* PickupComponent);
	void BindUpdateWidgetCallbacks(UPickupComponent* PickupComponent);

};
