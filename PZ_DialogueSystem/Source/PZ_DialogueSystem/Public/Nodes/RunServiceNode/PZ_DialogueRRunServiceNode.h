// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"
#include "PZ_DialogueRRunServiceNode.generated.h"



class UPZ_DialogueService;



UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_DialogueRRunServiceNode : public UPZ_DialogueRBaseNode
{
	GENERATED_BODY()


//Constructor
public:

	UPZ_DialogueRRunServiceNode();




//>>............................................................................................................<<//


//Blueprint methods
public:

	virtual bool Execute_Implementation(UPZ_DialogueContext* DialogueContext) override;


//Blueprint values
public:

	/*
		Array of services which will be activated.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
		TArray<UPZ_DialogueService*> Services;


//only for editor settings
#if WITH_EDITORONLY_DATA


#endif

};
