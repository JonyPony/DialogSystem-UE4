// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"



class FPZ_DialogueEditorStyle
{

public:

	static void Initialize();
	static void Shutdown();


	static const FName& GetStyleSetName() { return StyleSet->GetStyleSetName(); }
	static TSharedPtr<ISlateStyle> Get() { return StyleSet; }

	template<typename WidgetStyleType>
	static const WidgetStyleType& GetWidgetStyle(const FName& PropertyName, const ANSICHAR* Specifier = nullptr)
	{
		return StyleSet->GetWidgetStyle<WidgetStyleType>(PropertyName, Specifier);
	}



public:

	static const FName NodeIcon_DefaultIcon;
	static const FName NodeIcon_RootNodeIcon;
	static const FName NodeIcon_EndNodeIcon;
	static const FName NodeIcon_SayNodeIcon;
	static const FName NodeIcon_SendEventNodeIcon;
	static const FName NodeIcon_SelectorNodeIcon;
	static const FName NodeIcon_DelayNodeIcon;
	static const FName NodeIcon_ServiceNodeIcon;

private:

	static TSharedPtr<FSlateStyleSet> StyleSet;

};
