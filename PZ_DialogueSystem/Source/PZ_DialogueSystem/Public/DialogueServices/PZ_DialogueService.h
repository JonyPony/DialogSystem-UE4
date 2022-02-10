// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PZ_DialogueService.generated.h"



class UPZ_DialogueContext;



/*
	Base class for dialog services.
*/
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class PZ_DIALOGUESYSTEM_API UPZ_DialogueService : public UObject
{
	GENERATED_BODY()


//Constructor
public:

	UPZ_DialogueService();



//c++ public methods
public:

	void BeginService(UPZ_DialogueContext* DialogueContext);
	void EndService(UPZ_DialogueContext* DialogueContext);

//c++ private methods
private:

	UFUNCTION()
		void TickUpdate();
	UFUNCTION()
		void UserUpdate();



//c++ private values
private:

	FTimerDelegate UpdateDelegate;
	FTimerHandle UpdateTimerHandle;

	UPZ_DialogueContext* MyDialogueContext = nullptr;


//>>..................................................................................................................<<//


//Blueprint protected methods
protected:

	UFUNCTION(BlueprintNativeEvent, Category = "DialogueSystem|DialogueService")
		void OnServiceBegin(UPZ_DialogueContext* DialogueContext);
		virtual void OnServiceBegin_Implementation(UPZ_DialogueContext* DialogueContext) {}

	UFUNCTION(BlueprintNativeEvent, Category = "DialogueSystem|DialogueService")
		void OnServiceEnd(UPZ_DialogueContext* DialogueContext);
		virtual void OnServiceEnd_Implementation(UPZ_DialogueContext* DialogueContext) {}

	UFUNCTION(BlueprintNativeEvent, Category = "DialogueSystem|DialogueService")
		void OnServiceUpdate(float DeltaTime);
		virtual void OnServiceUpdate_Implementation(float DeltaTime) {}



	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueService")
		void StopThisService();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueService")
		UPZ_DialogueContext* GetDialogueContext() const;


//Blueprint values
public:

	/*
		<= 0 means every tick.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float UpdateInterval = 0.0f;

};
