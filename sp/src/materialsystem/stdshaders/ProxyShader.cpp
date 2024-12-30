#include "BaseVSShader.h"
#include "vertexlitgeneric_dx9_helper.h" // Include for the target shader

BEGIN_VS_SHADER(ProxyShader, "Proxy to redirect shaders") 

    BEGIN_SHADER_PARAMS
        SHADER_PARAM(REDIRECT_SHADER, SHADER_PARAM_TYPE_STRING, "VertexLitGeneric_SDK", "Target shader to redirect to")
    END_SHADER_PARAMS

    SHADER_INIT_PARAMS()
    {
        // Forward shader parameters to the target shader
        const char* redirectShaderName = params[REDIRECT_SHADER]->GetStringValue();
        if (!redirectShaderName || !redirectShaderName[0]) {
            params[REDIRECT_SHADER]->SetStringValue("VertexLitGeneric_SDK");
        }
    }

    SHADER_INIT
    {
        // Use the target shader's initialization logic
        const char* redirectShaderName = params[REDIRECT_SHADER]->GetStringValue();
        IMaterialVar** proxyParams = params;
        InitParams_VertexLitGeneric_DX9(proxyParams, pMaterialName);
    }

    SHADER_FALLBACK
    {
        // Use the target shader's fallback logic
        return "VertexLitGeneric"; // Fallback to something safe
    }

    SHADER_DRAW
    {
        const char* redirectShaderName = params[REDIRECT_SHADER]->GetStringValue();

        // Use the target shader's drawing logic
        DrawUsingVertexLitGeneric(pShaderAPI, pShaderShadow, params, vertexCompression);
    }

END_SHADER