// Shader CPP Integration File
#include "BaseVSShader.h"
#include "cpp_shader_constant_register_map.h"

#include "black_ps20.inc"
#include "black_vs20.inc"

BEGIN_VS_SHADER( Black, "Completely Black Shader" )

    BEGIN_SHADER_PARAMS
        // Add shader parameters here if needed
    END_SHADER_PARAMS

    SHADER_INIT
    {
        // Load textures or initialize constants if necessary
    }

    SHADER_FALLBACK
    {
        return 0; // No fallback
    }

    SHADER_DRAW
    {
        SHADOW_STATE
        {
            pShaderShadow->SetDefaultState();

            // Vertex format (position only, no textures or colors required)
            int vertexFormat = VERTEX_POSITION;
            pShaderShadow->VertexShaderVertexFormat( vertexFormat, 1, 0, 0 );

            // Load shaders
            DECLARE_STATIC_VERTEX_SHADER( black_vs20 );
			SET_STATIC_VERTEX_SHADER(black_vs20);

			DECLARE_STATIC_PIXEL_SHADER(black_ps20);
			SET_STATIC_PIXEL_SHADER(black_ps20);
        }

        DYNAMIC_STATE
        {
            // Set dynamic constants or states if necessary
			DECLARE_DYNAMIC_VERTEX_SHADER(black_vs20);
			SET_DYNAMIC_VERTEX_SHADER(black_vs20);

			DECLARE_DYNAMIC_PIXEL_SHADER(black_ps20);
			SET_DYNAMIC_PIXEL_SHADER(black_ps20);
        }

        Draw();
    }

END_SHADER