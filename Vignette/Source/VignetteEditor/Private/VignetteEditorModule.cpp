// Copyright Epic Games, Inc. All Rights Reserved.

#include "VignetteEditorModule.h"

#include "Interfaces/IPluginManager.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Docking/LayoutExtender.h"
#include "LevelEditor.h"

#include "VignetteParamsEditorWindow.h"

#define LOCTEXT_NAMESPACE "FVignetteModule"

void FVignetteEditorModule::StartupModule()
{
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner("VignetteEditorTab", FOnSpawnTab::CreateRaw(this, &FVignetteEditorModule::OnSpawnPluginTab))
        .SetDisplayName(NSLOCTEXT("Vignette Params Editor", "Vignette Params Editor", "Vignette Params Editor"))
        .SetMenuType(ETabSpawnerMenuType::Hidden);

    // We use this to add it to the level editor
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FVignetteEditorModule::AddMenuExtension));
}

void FVignetteEditorModule::AddMenuExtension()
{
    // Add the window to the Menu bar
    // Get the ToolMenu for the main Window menu
    UToolMenu* WindowMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
    FToolMenuSection& Section = WindowMenu->AddSection("SandboxTools", NSLOCTEXT("SandboxTools", "SandboxToolsSection", "SandboxTools"));
    Section.AddMenuEntry(
        "OpenVignetteEditor",
        NSLOCTEXT("VignetteEditor", "VignetteEditor", "Open Vignette Editor"),
        NSLOCTEXT("VignetteEditor", "VignetteEditorTooltip", "Launch the Vignette Editor window"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.GameSettings"),
        FUIAction(FExecuteAction::CreateRaw(this, &FVignetteEditorModule::OpenVignetteEditor))
    );
}

TSharedRef<SDockTab> FVignetteEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SVignetteParamsEditorWindow)
        ];
}

void FVignetteEditorModule::OpenVignetteEditor()
{
    FGlobalTabmanager::Get()->TryInvokeTab(FTabId("VignetteEditorTab"));
}

void FVignetteEditorModule::ShutdownModule()
{
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("VignetteEditorTab");

    UToolMenus::UnregisterOwner(this);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVignetteEditorModule, Vignette)