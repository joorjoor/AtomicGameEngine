
#ifndef COMPILEGS //ONLY IF NOT GS Shader

#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Fog.glsl"

#endif //ONLY IF NOT GS Shader

//---------------------------------------------VS-----------------------------------------------------
#ifdef COMPILEVS

out vec2 vTexCoord;
//out vec4 vWorldPos;
out vec3 vNormal;

#ifdef VERTEXCOLOR
    out vec4 vColor;
#endif

void VS()
{
    mat4 MV =  cModel * cView;
    gl_Position = iPos * MV;
    vNormal = iNormal;
    vTexCoord = GetTexCoord(iTexCoord);
    
    //mat4 modelMatrix = iModelMatrix;
    //vec3 worldPos = GetWorldPos(modelMatrix);
    //gl_Position = GetClipPos(worldPos);
    //vNormal = GetWorldNormal(modelMatrix);
    //vTexCoord = GetTexCoord(iTexCoord);
    //vWorldPos = vec4(worldPos, GetDepth(gl_Position));

    #ifdef VERTEXCOLOR
        vColor = iColor;
    #endif
}
#endif // COMPILEVS

//---------------------------------------------GS-----------------------------------------------------
#ifdef COMPILEGS

#include "Uniforms.glsl"
#include "Transform.glsl"

uniform mat4 cModel;
uniform mat4 cViewProj;
uniform vec4 cDepthMode;
uniform float cOffset;
uniform float cAngle;
uniform vec3 cCameraPos;
uniform vec3 cCameraRot;
uniform mat4 cProj;

//layout(triangles) in;
//layout(triangle_strip, max_vertices = 3) out;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;   

in vec2 vTexCoord[];
//in vec4 vWorldPos[];
in vec3 vNormal[];

out vec2 gTexCoord;
out vec4 gWorldPos;
out vec3 gNormal;

mat3 GetNormalMatrix(mat4 modelMatrix)
{
    return mat3(modelMatrix[0].xyz, modelMatrix[1].xyz, modelMatrix[2].xyz);
}

float GetDepth(vec4 clipPos)
{
    return dot(clipPos.zw, cDepthMode.zw);
}
#line 81
void GS()
{ 
    mat4 modelMatrix = cModel;
    vec2 quad[4];
    vec2 tc[4];
    
    mat2 RotationMatrix = mat2( cos( cAngle ), -sin( cAngle ),
                                sin( cAngle ),  cos( cAngle ));
    
    tc[0] = vec2(0,0); //left-bottom 
    tc[1] = vec2(0,1); //left-top
    tc[2] = vec2(1,0); //right-bottom
    tc[3] = vec2(1,1); //right-top
          
    vec4 iPos = gl_in[0].gl_Position;
    vec3 faceNormal = normalize(vNormal[0] * GetNormalMatrix(modelMatrix));
    
    quad[0] = (iPos.xy + (vec2(-0.5, -0.5) * RotationMatrix) * cOffset);
    quad[1] = (iPos.xy + (vec2(-0.5, 0.5) * RotationMatrix) * cOffset);
    quad[2] = (iPos.xy + (vec2(0.5, -0.5)* RotationMatrix) * cOffset);
    quad[3] = (iPos.xy + (vec2(0.5, 0.5) * RotationMatrix) * cOffset);
    
    for ( int i = 0; i < 4; i++ )
    {
        vec4 quadVertexWp = vec4(quad[i].xy, iPos.zw);
        gl_Position = quadVertexWp * cProj;
        gNormal = faceNormal;
        gWorldPos = vec4(quadVertexWp.xyz, GetDepth(gl_Position));
        gTexCoord = tc[i].xy;
        EmitVertex ();
    }
    EndPrimitive ();    
}
#endif // COMPILEGS
//---------------------------------------------PS-----------------------------------------------------
#ifdef COMPILEPS

in vec2 gTexCoord;
in vec4 gWorldPos;
in vec3 gNormal;

#ifdef VERTEXCOLOR
    varying vec4 gColor;
#endif

void PS()
{
    // Get material diffuse albedo
    #ifdef DIFFMAP
        vec4 diffColor = cMatDiffColor * texture2D(sDiffMap, gTexCoord);
        #ifdef ALPHAMASK
            if (diffColor.a < 0.5)
                discard;
        #endif
    #else
        vec4 diffColor = cMatDiffColor;
    #endif

    #ifdef VERTEXCOLOR
        diffColor *= gColor;
    #endif

    // Get fog factor
    #ifdef HEIGHTFOG
        float fogFactor = GetHeightFogFactor(gWorldPos.w, gWorldPos.y);
    #else
        float fogFactor = GetFogFactor(gWorldPos.w);
    #endif

    #if defined(PREPASS)
        // Fill light pre-pass G-Buffer
        gl_FragData[0] = vec4(0.5, 0.5, 0.5, 1.0);
        gl_FragData[1] = vec4(EncodeDepth(gWorldPos.w), 0.0);
    #elif defined(DEFERRED)
        gl_FragData[0] = vec4(GetFog(diffColor.rgb, fogFactor), diffColor.a);
        gl_FragData[1] = vec4(0.0, 0.0, 0.0, 0.0);
        gl_FragData[2] = vec4(0.5, 0.5, 0.5, 1.0);
        gl_FragData[3] = vec4(EncodeDepth(gWorldPos.w), 0.0);
    #else
        #ifndef MRT1
            gl_FragColor = vec4(GetFog(diffColor.rgb, fogFactor), diffColor.a);
        #else
            gl_FragData[0] = vec4(GetFog(diffColor.rgb, fogFactor), diffColor.a);
            gl_FragData[1] = vec4(1.0);
        #endif
    #endif
}

#endif
