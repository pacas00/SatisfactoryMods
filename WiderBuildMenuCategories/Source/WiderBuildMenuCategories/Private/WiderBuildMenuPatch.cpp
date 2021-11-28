// Copyright Coffee Stain Studios. All Rights Reserved.


#pragma once

#include "WiderBuildMenuPatch.h"

#include "Patching/BlueprintHookHelper.h"
#include "Patching/BlueprintHookManager.h"
#include "Reflection/ReflectionHelper.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/NamedSlot.h"
#include "Components/Overlay.h"
#include "Components/PanelWidget.h"
#include "Components/SizeBox.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(LogWBM);

void UWiderBuildMenuPatch::Log(FString string)
{
	UE_LOG(LogWBM, Log, TEXT("%s"), *string);
}

void UWiderBuildMenuPatch::PatchBuildMenu()
{
	//UClass* BuildMenuWidgetClass = LoadObject<UClass>(nullptr, TEXT("/Game/FactoryGame/Interface/UI/InGame/BuildMenu/Prototype/Widget_BuildMenu_SubCategoryButtons_Container.Widget_BuildMenu_SubCategoryButtons_Container_C"));
	UClass* BuildMenuWidgetClass = LoadObject<UClass>(nullptr, TEXT("/Game/FactoryGame/Interface/UI/InGame/BuildMenu/Prototype/Widget_BuildMenu.Widget_BuildMenu_C"));

	checkf(BuildMenuWidgetClass, TEXT("Build Menu Widget Not found"));

	UFunction* ConstructFunction = BuildMenuWidgetClass->FindFunctionByName(TEXT("Construct"));
	checkf(ConstructFunction, TEXT("Function ConstructFunction Not found"));

	if (ConstructFunction)
	{
		UBlueprintHookManager* HookManager = GEngine->GetEngineSubsystem<UBlueprintHookManager>();
		HookManager->HookBlueprintFunction(ConstructFunction, [](FBlueprintHookHelper& HookHelper) {
	        UWiderBuildMenuPatch::ApplyPatch(CastChecked<UUserWidget>(HookHelper.GetContext()));
	    }, EPredefinedHookOffset::Return);
	}
}

void UWiderBuildMenuPatch::ApplyPatch(UUserWidget* BuildMenuBrowserWidget) {

	Log(BuildMenuBrowserWidget->GetRootWidget()->GetName()); //Overlay_1
	//UPanelWidget UCanvasPanel UHorizontalBox UOverlay UScrollBox UWidgetSwitcher
	
	//Step 1, Work our way from the build menu into the BuildMenu_Browser

	UOverlay* Overlay_1 = Cast<UOverlay>(BuildMenuBrowserWidget->GetRootWidget());
	UUserWidget* Widget_Window_DarkMode = Cast<UUserWidget>(Overlay_1->GetChildAt(0));
	UNamedSlot* WindowBody = FReflectionHelper::GetObjectPropertyValue<UNamedSlot>(Widget_Window_DarkMode, TEXT("WindowBody"));
	UUserWidget* Widget_SlidingTabs = Cast<UUserWidget>(WindowBody->GetContent());	
	UNamedSlot* mContent = FReflectionHelper::GetObjectPropertyValue<UNamedSlot>(Widget_SlidingTabs, TEXT("mContent"));
	UHorizontalBox* HorizontalBox_2 = Cast<UHorizontalBox>(mContent->GetContent());	
	
	UUserWidget* mBrowser_Builder = Cast<UUserWidget>(HorizontalBox_2->GetChildAt(0)); //Build Menu BPW_BuildMenu_Browser
	Log(TEXT("6"));
	Log(mBrowser_Builder->GetName());

	UUserWidget* BPW_Customiser = Cast<UUserWidget>(HorizontalBox_2->GetChildAt(1));
	Log(TEXT("7"));
	Log(BPW_Customiser->GetName());

	//Step Two, Get to work on the build menu.
	UPanelWidget* CanvasPanel_0 = Cast<UPanelWidget>(mBrowser_Builder->GetRootWidget());
	UUserWidget* Widget_Window_AlternateContentBackground_DarkMode_C_0 = Cast<UUserWidget>(CanvasPanel_0->GetChildAt(0));
	USizeBox* SizeBox_0 = Cast<USizeBox>(CanvasPanel_0->GetChildAt(1));
	UHorizontalBox* HorizontalBox_1 = Cast<UHorizontalBox>(CanvasPanel_0->GetChildAt(2));

	UCanvasPanelSlot* HorizontalBox_1_slot = Cast<UCanvasPanelSlot>(HorizontalBox_1->Slot);
	HorizontalBox_1_slot->SetPosition(FVector2D(150,0));

	UCanvasPanelSlot* SizeBox_0_slot = Cast<UCanvasPanelSlot>(SizeBox_0->Slot);
	SizeBox_0_slot->SetSize(FVector2D(150, 0));

	UCanvasPanelSlot* Widget_Window_AlternateContentBackground_DarkMode_C_0_slot = Cast<UCanvasPanelSlot>(Widget_Window_AlternateContentBackground_DarkMode_C_0->Slot);
	Widget_Window_AlternateContentBackground_DarkMode_C_0_slot->SetSize(FVector2D(150, 0));


	//Step Three. Customiser has a BPW_BuildMenu_Browser too!
	UPanelWidget* CanvasPanel_Cust = Cast<UPanelWidget>(BPW_Customiser->GetRootWidget());
	UUserWidget* Cust_bm_browser = Cast<UUserWidget>(CanvasPanel_Cust->GetChildAt(0));


	//Step Four, Do it again!
	CanvasPanel_0 = Cast<UPanelWidget>(Cust_bm_browser->GetRootWidget());
	Widget_Window_AlternateContentBackground_DarkMode_C_0 = Cast<UUserWidget>(CanvasPanel_0->GetChildAt(0));
	SizeBox_0 = Cast<USizeBox>(CanvasPanel_0->GetChildAt(1));
	HorizontalBox_1 = Cast<UHorizontalBox>(CanvasPanel_0->GetChildAt(2));

	HorizontalBox_1_slot = Cast<UCanvasPanelSlot>(HorizontalBox_1->Slot);
	HorizontalBox_1_slot->SetPosition(FVector2D(150,0));

	SizeBox_0_slot = Cast<UCanvasPanelSlot>(SizeBox_0->Slot);
	SizeBox_0_slot->SetSize(FVector2D(150, 0));

	Widget_Window_AlternateContentBackground_DarkMode_C_0_slot = Cast<UCanvasPanelSlot>(Widget_Window_AlternateContentBackground_DarkMode_C_0->Slot);
	Widget_Window_AlternateContentBackground_DarkMode_C_0_slot->SetSize(FVector2D(150, 0));

}