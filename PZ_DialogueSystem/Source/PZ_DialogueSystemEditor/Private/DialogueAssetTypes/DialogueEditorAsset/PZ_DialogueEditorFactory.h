// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "PZ_DialogueEditorFactory.generated.h"



UCLASS()
class UPZ_DialogueEditorFactory : public UFactory
{
	GENERATED_BODY()

public:

	UPZ_DialogueEditorFactory();



public:

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
