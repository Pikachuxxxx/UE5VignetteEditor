#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UVignetteSubSystem;

class SVignetteParamsEditorWindow : public SCompoundWidget
{
public:
    // for customizing editor window looks
    SLATE_BEGIN_ARGS(SVignetteParamsEditorWindow) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    UVignetteSubSystem* VignetteSubsystem;
};