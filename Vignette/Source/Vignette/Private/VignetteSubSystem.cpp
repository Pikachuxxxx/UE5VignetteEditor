#include "VignetteSubSystem.h"

#include "SceneViewExtension.h"

#include "VignetteViewExtension.h"

void UVignetteSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Display, TEXT("[Vignette] Loading Vignette plugin..."));
    UE_LOG(LogTemp, Warning, TEXT("[Vignette] View SubSystem Init..."));

    VignetteViewExtension = FSceneViewExtensions::NewExtension<FVignetteViewExtension>();
}

void UVignetteSubSystem::Deinitialize()
{
    UE_LOG(LogTemp, Warning, TEXT("[Vignette] Deinitilalizing View SubSystem..."));

    // Prevent this SVE from being gathered, in case it is kept alive by a strong reference somewhere else.
    {
        VignetteViewExtension->IsActiveThisFrameFunctions.Empty();

        FSceneViewExtensionIsActiveFunctor IsActiveFunctor;

        IsActiveFunctor.IsActiveFunction = [](const ISceneViewExtension* SceneViewExtension, const FSceneViewExtensionContext& Context)
            {
                return TOptional<bool>(false);
            };

        VignetteViewExtension->IsActiveThisFrameFunctions.Add(IsActiveFunctor);
    }

    ENQUEUE_RENDER_COMMAND(ReleaseSVE)([this](FRHICommandListImmediate& RHICmdList)
        {
            // Prevent this SVE from being gathered, in case it is kept alive by a strong reference somewhere else.
            {
                VignetteViewExtension->IsActiveThisFrameFunctions.Empty();

                FSceneViewExtensionIsActiveFunctor IsActiveFunctor;

                IsActiveFunctor.IsActiveFunction = [](const ISceneViewExtension* SceneViewExtension, const FSceneViewExtensionContext& Context)
                    {
                        return TOptional<bool>(false);
                    };

                VignetteViewExtension->IsActiveThisFrameFunctions.Add(IsActiveFunctor);
            }

            VignetteViewExtension.Reset();
            VignetteViewExtension = nullptr;
        });

    // Finish all rendering commands before cleaning up actors.
    FlushRenderingCommands();

    UE_LOG(LogTemp, Display, TEXT("[Vignette] Unloading Vignette plugin..."));
}
