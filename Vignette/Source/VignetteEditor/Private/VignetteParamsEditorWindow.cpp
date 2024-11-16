#include "VignetteParamsEditorWindow.h"

#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "SubSystems/EngineSubsystem.h"

#include "VignetteSubSystem.h"

void SVignetteParamsEditorWindow::Construct(const FArguments& InArgs)
{
    VignetteSubsystem = GEngine->GetEngineSubsystem<UVignetteSubSystem>();

    ChildSlot
        [
            SNew(SBorder)
                .Padding(10)
                .BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
                [
                    SNew(SVerticalBox)

                        // Editable number entry box for VigRadius (float)
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        .Padding(5)
                        [
                            SNew(STextBlock)
                                .Text(NSLOCTEXT("HelloTriangleEditor", "VigRadiusLabel", "Vignette Radius"))
                        ]
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(SEditableTextBox)
                                .Text(FText::FromString(FString::Printf(TEXT("%.2f"), VignetteSubsystem->VigRadius))) // Display current value as text
                                .OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
                                    {
                                        // Convert the entered text to a float and update VigRadius
                                        VignetteSubsystem->VigRadius = FCString::Atof(*NewText.ToString());
                                    })
                        ]

                        // Editable number entry box for VigIntensity (int)
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        .Padding(5)
                        [
                            SNew(STextBlock)
                                .Text(NSLOCTEXT("HelloTriangleEditor", "VigIntensityLabel", "Vignette Intensity"))
                        ]
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(SEditableTextBox)
                                .Text(FText::FromString(FString::Printf(TEXT("%d"), VignetteSubsystem->VigIntensity))) // Display current value as text
                                .OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
                                    {
                                        // Convert the entered text to an integer and update VigIntensity
                                        VignetteSubsystem->VigIntensity = FCString::Atoi(*NewText.ToString());
                                    })
                        ]
                ]
        ];
}
