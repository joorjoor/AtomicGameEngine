#ifndef COMPILEGS //ONLY IF NOT GS Shader

#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"
#include "Fog.glsl"

#endif //ONLY IF NOT GS Shader

//---------------------------------------------VS-----------------------------------------------------
#ifdef COMPILEVS

    #ifdef NORMALMAP
        out vec4 vTexCoord;
        out vec4 vTangent;
    #else
        out vec2 vTexCoord;
    #endif
    out vec3 vNormal;
    out vec4 vWorldPos;
    #ifdef VERTEXCOLOR
        out vec4 vColor;
    #endif
    #ifdef PERPIXEL
        #ifdef SHADOW
            out mat4 vShadowPos;
        #endif
        #ifdef SPOTLIGHT
            out vec4 vSpotPos;
        #endif
        #ifdef POINTLIGHT
            out vec3 vCubeMaskVec;
        #endif
    #else
        out vec3 vVertexLight;
        out vec4 vScreenPos;
        #ifdef ENVCUBEMAP
            out vec3 vReflectionVec;
        #endif
        #if defined(LIGHTMAP) || defined(AO)
            out vec2 vTexCoord2;
        #endif
    #endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vNormal = GetWorldNormal(modelMatrix);
    vWorldPos = vec4(worldPos, GetDepth(gl_Position));

    #ifdef VERTEXCOLOR
        vColor = iColor;
    #endif

    #ifdef NORMALMAP
        vec3 tangent = GetWorldTangent(modelMatrix);
        vec3 bitangent = cross(tangent, vNormal) * iTangent.w;
        vTexCoord = vec4(GetTexCoord(iTexCoord), bitangent.xy);
        vTangent = vec4(tangent, bitangent.z);
    #else
        vTexCoord = GetTexCoord(iTexCoord);
    #endif

    #ifdef PERPIXEL
        // Per-pixel forward lighting
        vec4 projWorldPos = vec4(worldPos, 1.0);

        #ifdef SHADOW
            // Shadow projection: transform from world space to shadow space
            for (int i=0; i < NUMCASCADES; i++) 
                vShadowPos[i] = GetShadowPos(i, projWorldPos);       
        #endif

        #ifdef SPOTLIGHT
            // Spotlight projection: transform from world space to projector texture coordinates
            vSpotPos = projWorldPos * cLightMatrices[0];
        #endif
    
        #ifdef POINTLIGHT
            vCubeMaskVec = (worldPos - cLightPos.xyz) * mat3(cLightMatrices[0][0].xyz, cLightMatrices[0][1].xyz, cLightMatrices[0][2].xyz);
        #endif
    #else
        // Ambient & per-vertex lighting
        #if defined(LIGHTMAP) || defined(AO)
            // If using lightmap, disregard zone ambient light
            // If using AO, calculate ambient in the PS
            vVertexLight = vec3(0.0, 0.0, 0.0);
            vTexCoord2 = iTexCoord2;
        #else
            vVertexLight = GetAmbient(GetZonePos(worldPos));
        #endif
        
        #ifdef NUMVERTEXLIGHTS
            for (int i = 0; i < NUMVERTEXLIGHTS; ++i)
                vVertexLight += GetVertexLight(i, worldPos, vNormal) * cVertexLights[i * 3].rgb;
        #endif
        
        vScreenPos = GetScreenPos(gl_Position);

        #ifdef ENVCUBEMAP
            vReflectionVec = worldPos - cCameraPos;
        #endif
    #endif
}
#endif

//---------------------------------------------GS-----------------------------------------------------
#ifdef COMPILEGS

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// Data From VS to GS
#ifdef NORMALMAP
    in vec4 vTexCoord[3];
    in vec4 vTangent[3];
#else
    in vec2 vTexCoord[3];
#endif
in vec3 vNormal[3];
in vec4 vWorldPos[3];
#ifdef VERTEXCOLOR
    in vec4 vColor[3];
#endif
#ifdef PERPIXEL
    #ifdef SHADOW
        in mat4 vShadowPos[3];
    #endif
    #ifdef SPOTLIGHT
        in vec4 vSpotPos[3];
    #endif
    #ifdef POINTLIGHT
        in vec3 vCubeMaskVec[3];
    #endif
#else
    in vec3 vVertexLight[3];
    in vec4 vScreenPos[3];
    #ifdef ENVCUBEMAP
        in vec3 vReflectionVec[3];
    #endif
    #if defined(LIGHTMAP) || defined(AO)
        in vec2 vTexCoord2[3];
    #endif
#endif
    
// Data from GS to PS
#ifdef NORMALMAP
    out vec4 gsTexCoord;
    out vec4 gsTangent;
#else
    out vec2 gsTexCoord;
#endif
out vec3 gsNormal;
out vec4 gsWorldPos;
#ifdef VERTEXCOLOR
   out vec4 gsColor;
#endif
#ifdef PERPIXEL
    #ifdef SHADOW
        out mat4 gsShadowPos;
    #endif
    #ifdef SPOTLIGHT
        out vec4 gsSpotPos;
    #endif
    #ifdef POINTLIGHT
        out vec3 gsCubeMaskVec;
    #endif
#else
    out vec3 gsVertexLight;
    out vec4 gsScreenPos;
    #ifdef ENVCUBEMAP
        out vec3 gsReflectionVec;
    #endif
    #if defined(LIGHTMAP) || defined(AO)
        out vec2 gsTexCoord2;
    #endif
#endif
out vec3 gsBarycentric;

void GS()
{ 
    // Each vertex in the mesh gets an additional attribute which name called barycentric
    vec3 bc[3];
    bc[0] = vec3(1,0,0); // first vertex
    bc[1] = vec3(0,1,0); // second
    bc[2] = vec3(0,0,1); // third
    
    for ( int i = 0; i < gl_in.length(); i++ )
    { 
        gsBarycentric = bc[i];
        gsNormal = vNormal[i];
        gsWorldPos = vWorldPos[i];
        #ifdef NORMALMAP
            gsTexCoord = vTexCoord[i];
            gsTangent = vTangent[i];
        #else
            gsTexCoord = vTexCoord[i];
        #endif
        
        #ifdef VERTEXCOLOR
            gsColor = vColor[i];    
        #endif
        
        #ifdef PERPIXEL
            #ifdef SHADOW
                gsShadowPos = vShadowPos[i];
            #endif
            #ifdef SPOTLIGHT
                gsSpotPos = vSpotPos[i];
            #endif
            #ifdef POINTLIGHT
                gsCubeMaskVec = vCubeMaskVec[i];
            #endif
        #else
            gsVertexLight = vVertexLight[i];
            gsScreenPos = vScreenPos[i];
            #ifdef ENVCUBEMAP
                gsReflectionVec = vReflectionVec[i];
            #endif
            #if defined(LIGHTMAP) || defined(AO)
                gsTexCoord2 = vTexCoord2[i];
            #endif
        #endif
        gl_Position = gl_in[i].gl_Position;
        gl_ClipDistance[i] = gl_in[i].gl_ClipDistance[i];     
        EmitVertex ();
    }
    EndPrimitive ();    
}
#endif // COMPILEGS


//---------------------------------------------PS-----------------------------------------------------
#ifdef COMPILEPS

#ifdef NORMALMAP
    in vec4 gsTexCoord;
    in vec4 gsTangent;
#else
    in vec2 gsTexCoord;
#endif
in vec3 gsNormal;
in vec4 gsWorldPos;
#ifdef VERTEXCOLOR
   in vec4 gsColor;
#endif
#ifdef PERPIXEL
    #ifdef SHADOW
        in mat4 gsShadowPos;
    #endif
    #ifdef SPOTLIGHT
        in vec4 gsSpotPos;
    #endif
    #ifdef POINTLIGHT
        in vec3 gsCubeMaskVec;
    #endif
#else
    in vec3 gsVertexLight;
    in vec4 gsScreenPos;
    #ifdef ENVCUBEMAP
        in vec3 gsReflectionVec;
    #endif
    #if defined(LIGHTMAP) || defined(AO)
        in vec2 gsTexCoord2;
    #endif
#endif
in vec3 gsBarycentric;

vec3 fwidth(vec3 vBC) 
{
    return vec3(abs(dFdx(vBC)) + abs(dFdy(vBC)));
}

float edgeFactor()
{
    vec3 d = fwidth(gsBarycentric);
    vec3 a3 = smoothstep(vec3(0.0), d*1.0, gsBarycentric);
    return min(min(a3.x, a3.y), a3.z);
}

void PS()
{
    // Get material diffuse albedo
    #ifdef DIFFMAP
        vec4 diffInput = texture2D(sDiffMap, gsTexCoord.xy);
        #ifdef ALPHAMASK
            if (diffInput.a < 0.5)
                discard;
        #endif
        vec4 diffColor = cMatDiffColor * diffInput;
    #else
        vec4 diffColor = cMatDiffColor;
    #endif

    #ifdef VERTEXCOLOR
        diffColor *= gsColor;
    #endif
    
    // Get material specular albedo
    #ifdef SPECMAP
        vec3 specColor = cMatSpecColor.rgb * texture2D(sSpecMap, gsTexCoord.xy).rgb;
    #else
        vec3 specColor = cMatSpecColor.rgb;
    #endif

    // Get normal
    #ifdef NORMALMAP
        mat3 tbn = mat3(gsTangent.xyz, vec3(gsTexCoord.zw, gsTangent.w), gsNormal);
        vec3 normal = normalize(tbn * DecodeNormal(texture2D(sNormalMap, gsTexCoord.xy)));
    #else
        vec3 normal = normalize(gsNormal);
    #endif

    // Get fog factor
    #ifdef HEIGHTFOG
        float fogFactor = GetHeightFogFactor(gsWorldPos.w, gsWorldPos.y);
    #else
        float fogFactor = GetFogFactor(gsWorldPos.w);
    #endif

    vec4 wireFrameColor = vec4(0,1,1,1) * 0.5;
            
    
    #if defined(PERPIXEL)
        // Per-pixel forward lighting
        vec3 lightColor;
        vec3 lightDir;
        vec3 finalColor;

        float diff = GetDiffuse(normal, gsWorldPos.xyz, lightDir);

        #ifdef SHADOW
            vec4 vShadowPos[NUMCASCADES];
            for (int i=0; i < NUMCASCADES; i++)
                vShadowPos[i] = gsShadowPos[i];
            
            diff *= GetShadow(vShadowPos, gsWorldPos.w);
        #endif
    
        #if defined(SPOTLIGHT)
            lightColor = gsSpotPos.w > 0.0 ? texture2DProj(sLightSpotMap, gsSpotPos).rgb * cLightColor.rgb : vec3(0.0, 0.0, 0.0);
        #elif defined(CUBEMASK)
            lightColor = textureCube(sLightCubeMap, gsCubeMaskVec).rgb * cLightColor.rgb;
        #else
            lightColor = cLightColor.rgb;
        #endif
    
        #ifdef SPECULAR
            float spec = GetSpecular(normal, cCameraPosPS - gsWorldPos.xyz, lightDir, cMatSpecColor.a);
            finalColor = diff * lightColor * (diffColor.rgb + spec * specColor * cLightColor.a);
        #else
            finalColor = diff * lightColor * diffColor.rgb;
        #endif

        #ifdef AMBIENT
            finalColor += cAmbientColor * diffColor.rgb;
            finalColor += cMatEmissiveColor;
            //gl_FragColor = vec4(GetFog(finalColor, fogFactor), diffColor.a);
            vec4 shadedColor = vec4(GetFog(finalColor, fogFactor), diffColor.a);

            gl_FragColor = mix(wireFrameColor, shadedColor, edgeFactor());
        #else
            vec4 shadedColor = vec4(GetFog(finalColor, fogFactor), diffColor.a);
            gl_FragColor = mix(wireFrameColor, shadedColor, edgeFactor());

            //gl_FragColor = vec4(GetLitFog(finalColor, fogFactor), diffColor.a);
        #endif
    #elif defined(PREPASS)
        // Fill light pre-pass G-Buffer
        float specPower = cMatSpecColor.a / 255.0;

        gl_FragData[0] = vec4(normal * 0.5 + 0.5, specPower);
        gl_FragData[1] = vec4(EncodeDepth(gsWorldPos.w), 0.0);
    #elif defined(DEFERRED)
        // Fill deferred G-buffer
        float specIntensity = specColor.g;
        float specPower = cMatSpecColor.a / 255.0;

        vec3 finalColor = gsVertexLight * diffColor.rgb;
        #ifdef AO
            // If using AO, the vertex light ambient is black, calculate occluded ambient here
            finalColor += texture2D(sEmissiveMap, gsTexCoord2).rgb * cAmbientColor * diffColor.rgb;
        #endif

        #ifdef ENVCUBEMAP
            finalColor += cMatEnvMapColor * textureCube(sEnvCubeMap, reflect(gsReflectionVec, normal)).rgb;
        #endif
        #ifdef LIGHTMAP
            finalColor += texture2D(sEmissiveMap, gsTexCoord2).rgb * diffColor.rgb;
        #endif
        #ifdef EMISSIVEMAP
            finalColor += cMatEmissiveColor * texture2D(sEmissiveMap, gsTexCoord.xy).rgb;
        #else
            finalColor += cMatEmissiveColor;
        #endif
        
        vec4 shadedColor = vec4(GetFog(finalColor, fogFactor), 1.0);
        gl_FragData[0] = mix(wireFrameColor, shadedColor, edgeFactor()); 
        gl_FragData[1] = fogFactor * vec4(diffColor.rgb, specIntensity);
        gl_FragData[2] = vec4(normal * 0.5 + 0.5, specPower);
        gl_FragData[3] = vec4(EncodeDepth(gsWorldPos.w), 0.0);
    #else
        // Ambient & per-vertex lighting
        vec3 finalColor = gsVertexLight * diffColor.rgb;
        #ifdef AO
            // If using AO, the vertex light ambient is black, calculate occluded ambient here
            finalColor += texture2D(sEmissiveMap, gsTexCoord2).rgb * cAmbientColor * diffColor.rgb;
        #endif
        
        #ifdef MATERIAL
            // Add light pre-pass accumulation result
            // Lights are accumulated at half intensity. Bring back to full intensity now
            vec4 lightInput = 2.0 * texture2DProj(sLightBuffer, gsScreenPos);
            vec3 lightSpecColor = lightInput.a * lightInput.rgb / max(GetIntensity(lightInput.rgb), 0.001);

            finalColor += lightInput.rgb * diffColor.rgb + lightSpecColor * specColor;
        #endif

        #ifdef ENVCUBEMAP
            finalColor += cMatEnvMapColor * textureCube(sEnvCubeMap, reflect(gsReflectionVec, normal)).rgb;
        #endif
        #ifdef LIGHTMAP
            finalColor += texture2D(sEmissiveMap, gsTexCoord2).rgb * diffColor.rgb;
        #endif
        #ifdef EMISSIVEMAP
            finalColor += cMatEmissiveColor * texture2D(sEmissiveMap, gsTexCoord.xy).rgb;
        #else
            finalColor += cMatEmissiveColor;
        #endif
        
        vec4 shadedColor = vec4(GetFog(finalColor, fogFactor), diffColor.a);
        gl_FragColor = mix(wireFrameColor, shadedColor, edgeFactor());
        //gl_FragColor = vec4(GetFog(finalColor, fogFactor), diffColor.a);
    #endif
}
#endif