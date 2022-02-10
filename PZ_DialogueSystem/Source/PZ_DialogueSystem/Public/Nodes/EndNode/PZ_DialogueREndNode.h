// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/BaseNode/PZ_DialogueRBaseNode.h"
#include "PZ_DialogueREndNode.generated.h"



UCLASS()
class PZ_DIALOGUESYSTEM_API UPZ_DialogueREndNode : public UPZ_DialogueRBaseNode
{
	GENERATED_BODY()


//Constructor
public:

	UPZ_DialogueREndNode();




//>>............................................................................................................<<//


//Blueprint methods
public:

	virtual bool Execute_Implementation(UPZ_DialogueContext* DialogueContext) override;


//Blueprint values
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FEventInfo> EndEvents;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool WillParticipantsAutoLeave = true;



//only for editor settings
#if WITH_EDITORONLY_DATA


#endif

};
