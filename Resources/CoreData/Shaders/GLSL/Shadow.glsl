#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"

#ifdef VSM_SHADOW
    varying vec3 vTexCoord;
#else
    varying vec2 vTexCoord;
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    #ifdef VSM_SHADOW
        vTexCoord = vec3(GetTexCoord(iTexCoord), gl_Position.z / gl_Position.w * 0.5 + 0.5);
    #else
        vTexCoord = GetTexCoord(iTexCoord);
    #endif
}

void PS()
{
    #ifdef ALPHAMASK
        float alpha = texture2D(sDiffMap, vTexCoord.xy).a;
        if (alpha < 0.5)
            discard;
    #endif

    #ifdef VSM_SHADOW
        float depth = vTexCoord.z;
        depth = depth * 0.5 + 0.5;
		float moment1 = depth;
		float moment2 = depth * depth;
		
        // Adjusting moments (this is sort of bias per pixel) using partial derivative
		float dx = dFdx(depth);
		float dy = dFdy(depth);
		moment2 += 0.25*(dx*dx+dy*dy);
        
        gl_FragColor = vec4(moment1, moment2, 0.0, 0.0);
    #else
        gl_FragColor = vec4(1.0);
    #endif
}
