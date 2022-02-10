// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"
#include "PZ_DialogueRSayNode.generated.h"



class UPZ_PhrasePropagation;
class UPZ_DialogueService;



UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_DialogueRSayNode : public UPZ_DialogueRBaseNode
{
	GENERATED_BODY()


//Constructor
public:

	UPZ_DialogueRSayNode();




//>>............................................................................................................<<//


//Blueprint methods
public:

	virtual bool Execute_Implementation(UPZ_DialogueContext* DialogueContext) override;
	virtual bool StopExecute_Implementation(UPZ_DialogueContext* DialogueContext) override;


//Blueprint values
public:

	/*
		Name of role in dialog who willsay this phrase.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName WhoSay;
	/*
		Shorted name of phrase. 
	*/
	UPROPERTY(EditDefaultsOnly, meta = (MultiLine = true), BlueprintReadOnly)
		FText ShortedPhrase;
	/*
		Phrase text.
	*/
	UPROPERTY(EditDefaultsOnly, meta = (MultiLine = true), BlueprintReadOnly)
		FText Phrase;
	/*
		Voice of this phrase.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class USoundBase* PhraseSound = nullptr;
	/*
		Can this phrase be skipped.
		If parent node has at least one phrase with false then will not let everyone skip the dialog at this moment.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool CanBeSkipped = true;
	/*
		Information of this phrase. Can be used for AI.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
		UPZ_PhrasePropagation* PhrasePropagation = nullptr;

	/*
		Option of selection target of this phrase.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EChoseOptionType SayOption;
	/*
		Available roles in dialog to target selection.
		Choice base on SayOption.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FName> SayToRoles;

	/*
		Array of services which will be active when say node is playing.
		They will be removed when next node activated.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
		TArray<UPZ_DialogueService*> Services;



//only for editor settings
#if WITH_EDITORONLY_DATA


#endif

};
