// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PZ_PhrasePropagation.generated.h"


class UPZ_DialogueContext;



UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class PZ_DIALOGUESYSTEM_API UPZ_PhrasePropagation : public UObject
{
	GENERATED_BODY()

	friend UPZ_DialogueContext;
	friend class UPZ_DialogueRSayNode;


//Constructor
public:

	UPZ_PhrasePropagation();




//c++ protected values
protected:

	UPZ_DialogueContext* DialogInstigator;



//>>................................................................................................<<//


//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
		UPZ_DialogueContext* GetDialogInstigator() const;


//Blueprint values
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PropagationRadius = 300.0f;

};
