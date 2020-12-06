// SHADER INFO : http://codeflow.org/entries/2012/aug/02/easy-wireframe-display-with-barycentric-coordinates/

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
out vec4 vWorldPos;
out vec3 vNormal;

#ifdef VERTEXCOLOR
    out vec4 vColor;
#endif

void VS()
{    
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vNormal = GetWorldNormal(modelMatrix);
    vTexCoord = GetTexCoord(iTexCoord);
    vWorldPos = vec4(worldPos, GetDepth(gl_Position));

    #ifdef VERTEXCOLOR
        vColor = iColor;
    #endif
}
#endif // COMPILEVS

//---------------------------------------------GS-----------------------------------------------------
#ifdef COMPILEGS

#include "Uniforms.glsl"
#include "Transform.glsl"

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 vTexCoord[];
in vec4 vWorldPos[];
in vec3 vNormal[];

out vec2 gTexCoord;
out vec4 gWorldPos;
out vec3 gNormal;
out vec3 gBarycentric;

void GS()
{ 
    vec3 bc[3];
    
    // Each vertex in the mesh gets an additional attribute which name called barycentric
    bc[0] = vec3(1,0,0); // first vertex
    bc[1] = vec3(0,1,0); // second
    bc[2] = vec3(0,0,1); // third
       
    for ( int i = 0; i < gl_in.length(); i++ )
    {
        gl_Position = gl_in[i].gl_Position;
        gNormal = vNormal[i];
        gWorldPos = vWorldPos[i];
        gTexCoord = vTexCoord[i];
        gBarycentric = bc[i];
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
in vec3 gBarycentric;

#ifdef VERTEXCOLOR
    varying vec4 gColor;
#endif


vec3 fwidth(vec3 vBC) 
{
    return vec3(abs(dFdx(vBC)) + abs(dFdy(vBC)));
}

float edgeFactor()
{
    vec3 d = fwidth(gBarycentric);
    vec3 a3 = smoothstep(vec3(0.0), d*1.5, gBarycentric);
    return min(min(a3.x, a3.y), a3.z);
}

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
    
    vec4 wireFrameColor = vec4(0,1,1,1);
    vec4 finalColor = vec4(GetFog(diffColor.rgb, fogFactor), diffColor.a);
    
    #if defined(PREPASS)
        // Fill light pre-pass G-Buffer
        gl_FragData[0] = vec4(0.5, 0.5, 0.5, 1.0);
        gl_FragData[1] = vec4(EncodeDepth(gWorldPos.w), 0.0);
    #elif defined(DEFERRED)
        //gl_FragData[0] = vec4(GetFog(diffColor.rgb, fogFactor), diffColor.a);
        gl_FragData[0] = mix(wireFrameColor, finalColor, edgeFactor());
        gl_FragData[1] = vec4(0.0, 0.0, 0.0, 0.0);
        gl_FragData[2] = vec4(0.5, 0.5, 0.5, 1.0);
        gl_FragData[3] = vec4(EncodeDepth(gWorldPos.w), 0.0);
    #else        
        gl_FragColor = mix(wireFrameColor, finalColor, edgeFactor());
        
        #if 0
            if (any(lessThan(gBarycentric, vec3(0.01)))) 
            {
                 gl_FragColor = vec4(vec3(0,1,1), 1.0);
            }
            else
            {
                gl_FragColor = vec4(GetFog(diffColor.rgb, fogFactor), diffColor.a);
            }
        #endif
    #endif
        
}

#endif
