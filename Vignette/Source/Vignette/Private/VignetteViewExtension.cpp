#include "VignetteViewExtension.h"

#include "VignetteShaders.h"
#include "VignetteSubSystem.h"

#include "ScreenPass.h"
#include "PixelShaderUtils.h"
#include "PostProcess/PostProcessing.h"
#include "DynamicResolutionState.h"
#include "FXRenderingUtils.h"
#include "RenderGraphUtils.h"
#include "PostProcess/PostProcessMaterialInputs.h"

// https://mcro.de/c/rdg

DEFINE_GPU_STAT(VignettePass)

// CVAR to control this from console commands
static TAutoConsoleVariable<int32> CVarEnableVignettePass(
    TEXT("r.VignettePass.Enable"),
    1,
    TEXT("Enable or disable Vignette Pass.\n")
    TEXT("0: Disable\n")
    TEXT("1: Enable"),
    ECVF_RenderThreadSafe);
// Usage:
// r.VignettePass.Enable 0   // Disable the render pass
// r.VignettePass.Enable 1   // Enable the render pass

FVignetteViewExtension::FVignetteViewExtension(const FAutoRegister& AutoRegister)
    : FSceneViewExtensionBase(AutoRegister)
{

}

void FVignetteViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
    UE_LOG(LogCore, Warning, TEXT("[Vignette] FVignetteViewExtension::PrePostProcessPass_RenderThread ..."));

    // NO dynamic cast is possible
    checkSlow(View.bIsViewInfo);
    const FIntRect Viewport = static_cast<const FViewInfo&>(View).ViewRect;

    // Get the scene texture
    Inputs.Validate();

    const FIntRect PrimaryViewRect = UE::FXRenderingUtils::GetRawViewRectUnsafe(View);

    FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, PrimaryViewRect);

    FScreenPassRenderTarget Output;
    if (!Output.IsValid())
    {
        Output = FScreenPassRenderTarget::CreateFromInput(GraphBuilder, SceneColor, View.GetOverwriteLoadAction(), TEXT("VignetteRenderTarget"));
    }

    const FScreenPassTextureViewport InputViewport(SceneColor);
    const FScreenPassTextureViewport OutputViewport(SceneColor);

    {
        RDG_EVENT_SCOPE(GraphBuilder, "VignettePass");
        RDG_GPU_STAT_SCOPE(GraphBuilder, VignettePass);

        FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
        // Shader Parameter Setup, nothing for VS
        FVignettePSParams* PixelShaderParams = GraphBuilder.AllocParameters<FVignettePSParams>();
        // Set them using editor, the editor talks to USubSystem and we the values from there
        UVignetteSubSystem* Subsystem = GEngine->GetEngineSubsystem<UVignetteSubSystem>();
        PixelShaderParams->Intensity = Subsystem ? Subsystem->VigIntensity : 25;
        PixelShaderParams->Radius = Subsystem ? Subsystem->VigRadius : 0.25f;

        PixelShaderParams->SceneTexture = SceneColor.Texture;
        FRHISamplerState* PointClampSampler = TStaticSamplerState<SF_Point, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
        PixelShaderParams->Vignette_Sampler = PointClampSampler;

        // Set the Render Target In this case is the Scene Color, clear the scene RT
        Output.LoadAction = ERenderTargetLoadAction::ELoad;
        PixelShaderParams->RenderTargets[0] = Output.GetRenderTargetBinding();

        TShaderMapRef<FVignettePS> PixelShader(GlobalShaderMap);
        check(PixelShader.IsValid());

        ClearUnusedGraphResources(PixelShader, PixelShaderParams); // ???

        // Adding the RDG pass
        if (CVarEnableVignettePass.GetValueOnGameThread()) {
            AddDrawScreenPass(GraphBuilder, RDG_EVENT_NAME("Vignette"), static_cast<const FViewInfo&>(View), OutputViewport, InputViewport, PixelShader, PixelShaderParams);

            // Copy output back to SceneColor
            AddDrawTexturePass(GraphBuilder, View, Output.Texture, SceneColor.Texture);
            // All remaining passes are load.
            Output.LoadAction = ERenderTargetLoadAction::ELoad;
        }
    }
}

//FScreenPassTexture FVignetteViewExtension::VignettePass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& InOutInputs)
//{
//    UE_LOG(LogCore, Warning, TEXT("[Vignette] FVignetteViewExtension::PrePostProcessPass_RenderThread ..."));
//
//    // NO dynamic cast is possible
//    checkSlow(View.bIsViewInfo);
//    const FIntRect Viewport = static_cast<const FViewInfo&>(View).ViewRect;
//
//    const FScreenPassTexture& SceneColor = FScreenPassTexture::CopyFromSlice(GraphBuilder, InOutInputs.GetInput(EPostProcessMaterialInput::SceneColor));
//    check(SceneColor.IsValid());
//
//    FScreenPassRenderTarget Output = InOutInputs.OverrideOutput;
//
//    // If the override output is provided, it means that this is the last pass in post processing.
//    if (!Output.IsValid())
//    {
//        Output = FScreenPassRenderTarget::CreateFromInput(GraphBuilder, SceneColor, View.GetOverwriteLoadAction(), TEXT("VignetteRenderTarget"));
//    }
//
//    const FScreenPassTextureViewport InputViewport(SceneColor);
//    const FScreenPassTextureViewport OutputViewport(SceneColor);
//
//    {
//        RDG_EVENT_SCOPE(GraphBuilder, "VignettePass");
//        RDG_GPU_STAT_SCOPE(GraphBuilder, VignettePass);
//
//        FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
//        // Shader Parameter Setup, nothing for VS
//        FVignettePSParams* PixelShaderParams = GraphBuilder.AllocParameters<FVignettePSParams>();
//        // TODO: Set them using editor
//        PixelShaderParams->Intensity = 25;
//        PixelShaderParams->Radius = 0.35f;
//
//        PixelShaderParams->SceneTexture = SceneColor.Texture;
//        FRHISamplerState* PointClampSampler = TStaticSamplerState<SF_Point, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
//        PixelShaderParams->Vignette_Sampler = PointClampSampler;
//
//        // Set the Render Target In this case is the Scene Color, clear the scene RT
//        Output.LoadAction = ERenderTargetLoadAction::ELoad;
//        PixelShaderParams->RenderTargets[0] = Output.GetRenderTargetBinding();
//
//        TShaderMapRef<FVignettePS> PixelShader(GlobalShaderMap);
//        check(PixelShader.IsValid());
//
//        ClearUnusedGraphResources(PixelShader, PixelShaderParams); // ???
//
//        // Adding the RDG pass
//        if (CVarEnableVignettePass.GetValueOnGameThread()) {
//            AddDrawScreenPass(GraphBuilder, RDG_EVENT_NAME("Vignette"), static_cast<const FViewInfo&>(View), OutputViewport, InputViewport, PixelShader, PixelShaderParams);
//
//            // Copy output back to SceneColor
//            AddDrawTexturePass(GraphBuilder, View, Output.Texture, SceneColor.Texture);
//            // All remaining passes are load.
//            Output.LoadAction = ERenderTargetLoadAction::ELoad;
//        }
//        else
//            Output.Texture = SceneColor.Texture;
//    }
//
//    return MoveTemp(Output);
//}
//
//
//void FVignetteViewExtension::SubscribeToPostProcessingPass(EPostProcessingPass PassId, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
//{
//    if (PassId == EPostProcessingPass::Tonemap)
//    {
//        InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FVignetteViewExtension::VignettePass_RenderThread));
//    }
//}
