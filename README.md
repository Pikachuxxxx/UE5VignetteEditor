# Vignette Post-Processing Effect and Editor Window (UE5)

This repository provides a **Vignette Post-Processing Effect** implemented using Unreal Engine 5's Rendering Dependency Graph (RDG) and an editor tool for configuring vignette parameters. The tool integrates into Unreal Engine's menu system, under the **"Window"** menu, and features an intuitive editor for real-time adjustments to vignette properties.

---

## Features
- **Custom Vignette Effect** using Unreal's RDG.
- **Editor Tool** for adjusting vignette properties:
  - **Vignette Radius** (float)
  - **Vignette Intensity** (int)
- Easy integration into the Unreal Engine editor.

---

## Installation
1. Clone this repository into your Unreal Engine project's `Plugins` folder.
2. Open your project and enable the plugin in the **Plugins** window.
3. Restart the editor to load the plugin.

---

## How to Register the Editor Window
The vignette editor window is added to the **Window** menu in the Unreal Editor. The following code registers the window:

```cpp
// Add the window to the Menu bar
UToolMenu* WindowMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
FToolMenuSection& Section = WindowMenu->AddSection("SandboxTools", NSLOCTEXT("SandboxTools", "SandboxToolsSection", "SandboxTools"));
Section.AddMenuEntry(
    "OpenVignetteEditor",
    NSLOCTEXT("VignetteEditor", "VignetteEditor", "Open Vignette Editor"),
    NSLOCTEXT("VignetteEditor", "VignetteEditorTooltip", "Launch the Vignette Editor window"),
    FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.GameSettings"),
    FUIAction(FExecuteAction::CreateRaw(this, &FVignetteEditorModule::OpenVignetteEditor))
);
```

This code registers a menu entry titled "Open Vignette Editor" under Window > SandboxTools. Selecting it launches the editor.

Building the Vignette Editor Window
The editor window is constructed using Unreal's Slate UI framework. Below is the code used for the UI:

```cpp
Copy code
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
                                .Text(NSLOCTEXT("VignetteEditor", "VigRadiusLabel", "Vignette Radius"))
                        ]
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(SEditableTextBox)
                                .Text(FText::FromString(FString::Printf(TEXT("%.2f"), VignetteSubsystem->VigRadius)))
                                .OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
                                    {
                                        VignetteSubsystem->VigRadius = FCString::Atof(*NewText.ToString());
                                    })
                        ]

                        // Editable number entry box for VigIntensity (int)
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        .Padding(5)
                        [
                            SNew(STextBlock)
                                .Text(NSLOCTEXT("VignetteEditor", "VigIntensityLabel", "Vignette Intensity"))
                        ]
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(SEditableTextBox)
                                .Text(FText::FromString(FString::Printf(TEXT("%d"), VignetteSubsystem->VigIntensity)))
                                .OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
                                    {
                                        VignetteSubsystem->VigIntensity = FCString::Atoi(*NewText.ToString());
                                    })
                        ]
                ]
        ];
}
```

## UI Features
- Vignette Radius:
- Displays the current radius value.
- Allows direct input for new values.
- Vignette Intensity:
- Displays the current intensity value.
- Allows direct input for new values.

## How to Use
Access the Editor Window:

Navigate to Window > SandboxTools > Open Vignette Editor in the Unreal Engine menu bar.

Adjust Parameters:
- Modify the Vignette Radius and Vignette Intensity directly in the editor.
- Changes are applied in real-time through the associated subsystem (UVignetteSubsystem).

## Screenshots

## Contributing
Feel free to submit pull requests or open issues to enhance this plugin.
