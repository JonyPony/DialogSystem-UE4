// Fill out your copyright notice in the Description page of Project Settings.


#include "PZ_DialogueEditorFactory.h"
#include "PZ_DialogueEditor.h"




UPZ_DialogueEditorFactory::UPZ_DialogueEditorFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UPZ_DialogueEditor::StaticClass();
}





UObject* UPZ_DialogueEditorFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UPZ_DialogueEditor>(InParent, Class, Name, Flags | RF_Transactional);
}
