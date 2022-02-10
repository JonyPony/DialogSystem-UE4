// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "PZ_DialogueSystemComponent.generated.h"



class UPZ_DialogueContext;
class UPZ_PhrasePropagation;
class UPZ_BaseParticipantNickName;

class UPZ_DialogueEditor;
class UPZ_DialogueRBaseNode;




UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PZ_DIALOGUESYSTEM_API UPZ_DialogueSystemComponent : public UAudioComponent
{
	GENERATED_BODY()

	friend UPZ_DialogueContext;


//Constructor
public:

	UPZ_DialogueSystemComponent();



//c++ public methods
public:


//c++ protected methods
protected:

	virtual void BeginPlay() override;


//c++ private methods
private:





//c++ public values
public:


//c++ protected values
protected:

	UPROPERTY()
		UPZ_DialogueContext* ParticipateInDialog = nullptr;
	FName MyCurrentRole = "None";


//c++ private values
private:

	TArray<UPZ_DialogueRBaseNode*> PostChosedActions;



//>>..........................................................................................................................<<//


//Blueprint Protected methods
protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogueSystem|DialogueSystemComponent")
		void OnEnterToDialog();
		virtual void OnEnterToDialog_Implementation() {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogueSystem|DialogueSystemComponent")
		void OnLeaveFromDialog(const FName& LastRole);
		virtual void OnLeaveFromDialog_Implementation(const FName& LastRole) {}


//Blueprint Public methods
public:

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueSystemComponent")
		bool JoinToDialog(UPZ_DialogueContext* Dialog);
	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueSystemComponent")
		bool StartDialogWith(UPZ_DialogueEditor* Dialog, TArray<AActor*> OtherParticipants, UPZ_DialogueContext*& CreatedDialog);
	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueSystemComponent")
		bool LeaveFromDialog();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogueSystem|DialogueSystemComponent")
		bool GetCanParticipateInDialog(const UPZ_DialogueContext* DialogContext) const;
		virtual bool GetCanParticipateInDialog_Implementation(const UPZ_DialogueContext* DialogContext) const { return true; };



	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogueSystem|DialogueSystemComponent")
		void SayPhrase(const FText& Phrase, USoundBase* PhraseSound, UPZ_PhrasePropagation* PhrasePropagation);
		virtual void SayPhrase_Implementation(const FText& Phrase, USoundBase* PhraseSound, UPZ_PhrasePropagation* PhrasePropagation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogueSystem|DialogueSystemComponent")
		void HearPhrase(bool ForMe, const FText& Phrase, UPZ_PhrasePropagation* PhrasePropagation);
		virtual void HearPhrase_Implementation(bool ForMe, const FText& Phrase, UPZ_PhrasePropagation* PhrasePropagation) {};

	/*
		You can use UAudioComponent methods:
		Stop()
		StopDelayed()
		IsPlaying()
	*/




	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogueSystem|DialogueSystemComponent")
		void ChoseNextDialogActions(const TArray<UPZ_DialogueRBaseNode*>& InNextActions, TArray<UPZ_DialogueRBaseNode*>& OutNextActions);
		virtual void ChoseNextDialogActions_Implementation(const TArray<UPZ_DialogueRBaseNode*>& InNextActions, TArray<UPZ_DialogueRBaseNode*>& OutNextActions);

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueSystemComponent")
		void PostActionChose(const TArray<UPZ_DialogueRBaseNode*>& ChosedActions); 




	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueSystemComponent")
		void FinishSaying();

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem|DialogueSystemComponent")
		void ShutDownAll();


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueSystemComponent")
		UPZ_DialogueContext* GetCurrentDialog() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueSystemComponent")
		UPZ_DialogueEditor* GetCurrentDialogEditor() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DialogueSystem|DialogueSystemComponent")
		FName GetCurrentRole() const;




//Blueprint values
public:

	/*
		NickName class in dialogs. Used for connecting to role.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UPZ_BaseParticipantNickName> NickName;

	/*
		it is installed by the system, but can be changed gameplay by the user.
	*/
	UPROPERTY(BlueprintReadWrite)
		bool IsMyPhraseCanBeSkipped = true;

};
