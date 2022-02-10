// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"
#include "PZ_DialogueRDelayNode.generated.h"



class UPZ_DialogueService;



UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_DialogueRDelayNode : public UPZ_DialogueRBaseNode
{
	GENERATED_BODY()


//Constructor
public:

	UPZ_DialogueRDelayNode();




//>>............................................................................................................<<//


//Blueprint methods
public:

	virtual bool Execute_Implementation(UPZ_DialogueContext* DialogueContext) override;
	virtual bool StopExecute_Implementation(UPZ_DialogueContext* DialogueContext) override;


//Blueprint values
public:

	/*
		Delay time in seconds.
		Dialogue graph will stop iterating.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
		float DelayTime = 1.0;
	/*
		Array of services which will be active when delay node is playing.
		They will be removed when time is out.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
		TArray<UPZ_DialogueService*> DelayServices;


//only for editor settings
#if WITH_EDITORONLY_DATA


#endif

};
