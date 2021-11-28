// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"


#include "WiderBuildMenuPatch.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWBM, Log, All);
/**
 * 
 */
UCLASS(Blueprintable)
class WIDERBUILDMENUCATEGORIES_API UWiderBuildMenuPatch : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "WiderBuildMenu")
    static void PatchBuildMenu();
	static void ApplyPatch(class UUserWidget* MainMenuWidget);
	static void Log(FString string);
};
