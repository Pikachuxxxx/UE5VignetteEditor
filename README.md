# Vignette Post-Processing Effect and Editor Window (UE5)

This repository provides a **Vignette Post-Processing Effect** plugin for Unreal Engine 5 using the Rendering Dependency Graph (RDG) and RHI interaface and an editor tool for configuring vignette parameters. The plugin also integrates a window into Unreal Engine's menu system, under the **"Window"** menu for adjustment of vignette properties.

Vignette source: [ShaderToy Vignette Effect](https://www.shadertoy.com/view/lsKSWR)

## Features
- **Custom Vignette Effect** using Unreal's RDG.
- **Editor Window** for adjusting vignette properties:
  - **Vignette Radius** (float)
  - **Vignette Intensity** (int)

## Installation
1. Clone this repository into your Unreal Engine project's `Plugins` folder.
2. Open your project and enable the plugin in the **Plugins** window.
3. Restart the editor to load the plugin.


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

## Building the Vignette Editor Window
The editor window is constructed using Unreal's Slate UI framework. Create a new class that derives from `SCompoundWidget`:

```cpp
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

VignetteSubsystem is a UEngineSubsystem that updates the Pixel ShaderParams. We store a reference for that in the editor plugin.

**CVar:** Both the vignette and editor plugin are bult together, use `r.Vignette.Enable 0/1` to enable/disable the render pass.

## How to Use
Navigate to Window > SandboxTools > Open Vignette Editor in the Unreal Engine menu bar.

Adjust Parameters:
- Modify the Vignette Radius and Vignette Intensity directly in the editor.
- Changes are applied in real-time through the associated subsystem (UVignetteSubsystem).

## Screenshots
![UE5VignetteEditor](https://github.com/user-attachments/assets/04aaa3e3-1c29-48bd-b14b-fc8d3f6af882)


## Contributing
Feel free to submit pull requests or open issues to enhance this plugin.
