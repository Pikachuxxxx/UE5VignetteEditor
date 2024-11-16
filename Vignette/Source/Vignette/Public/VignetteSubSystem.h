#pragma once

#include "CoreMinimal.h"
#include "SubSystems/EngineSubsystem.h"

#include "VignetteSubSystem.generated.h" //???

class FVignetteViewExtension;

UCLASS()
class VIGNETTE_API UVignetteSubSystem : public UEngineSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    float VigRadius = 0.25f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    int VigIntensity = 25;
    //UPROPERTY(EditAnywhere, Config, Category = "Vignette")
    //float VigRandomVariableTest = 0.25f;

protected:
    TSharedPtr<FVignetteViewExtension, ESPMode::ThreadSafe> VignetteViewExtension;
};
