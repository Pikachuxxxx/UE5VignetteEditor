#pragma once

#include "GlobalShader.h"
#include "Runtime/Renderer/Public/ScreenPass.h"

/**
 * This file defines all the resources needed for rendering Vignette PPFX
 */

BEGIN_SHADER_PARAMETER_STRUCT(FVignettePSParams, )
    SHADER_PARAMETER(int, Intensity)
    SHADER_PARAMETER(float, Radius)
    SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneTexture)
    SHADER_PARAMETER_SAMPLER(SamplerState, Vignette_Sampler)
    RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()
class FVignettePS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FVignettePS);
    using FParameters = FVignettePSParams;
    SHADER_USE_PARAMETER_STRUCT(FVignettePS, FGlobalShader)
};
//----------------------------------------------------------------
// Define shaders

IMPLEMENT_SHADER_TYPE(, FVignettePS, TEXT("/VignetteShaders/Private/Vignette.usf"), TEXT("MainPS"), SF_Pixel);

//----------------------------------------------------------------

