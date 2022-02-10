// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PZ_DialogueManager.generated.h"



class UPZ_DialogueContext;
class UPZ_DialogueEditor;



UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_DialogueManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem")
		static UPZ_DialogueContext* CreateDialogContext(UPZ_DialogueEditor* Dialog, AActor* Outer);

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem")
		static bool CreateDialog(UPZ_DialogueEditor* Dialog, const TArray<AActor*>& Participants, UPZ_DialogueContext*& CreatedDialog);


	UFUNCTION(BlueprintCallable, Category = "DialogueSystem")
		static bool TryCreateDialogWithActorsInRadius
		(
			UPZ_DialogueEditor* Dialog,
			AActor* Outer, FVector Location, float Radius,
			const TArray<AActor*>& IgnoreActors,
			UPZ_DialogueContext*& CreatedDialog
		);
	
};
