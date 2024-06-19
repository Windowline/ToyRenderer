#include "SSAOShaderTmp.hpp"
#include "GLUtilGeometry.hpp"

const char* vertexSSAOTmp = R(
        in vec2 a_position;
        in vec2 a_texCoord;
        out vec2 v_texCoord;

        void main()
        {
           v_texCoord = a_texCoord;
           gl_Position = vec4(a_position, 0.0, 1.0);
        }
);

const char* fragmentSSAOTmp = R(
        uniform sampler2D u_posTexture;
        uniform sampler2D u_normalTexture;
        uniform sampler2D u_noiseTexture;  // 접선공간상의 임의 회전
        uniform vec3 u_samples[64];
        uniform vec2 u_screenSize;
        uniform mat4 u_viewMat;
        uniform mat4 u_projMat;

        in vec2 v_texCoord;
        layout (location = 0) out vec4 fragColor; //todo: float

        void main()
        {
            vec2 noiseScale = u_screenSize / 4.0;
            vec3 worldPos = texture(u_posTexture, v_texCoord).xyz;
            vec3 viewPos = (u_viewMat * vec4(worldPos, 1.0)).xyz;
            vec3 randomVec = texture(u_noiseTexture, v_texCoord * noiseScale).rgb;
            vec3 N = texture(u_normalTexture, v_texCoord).rgb;
            mat4 normalViewMat = transpose(inverse(u_viewMat));
            N = (normalViewMat * vec4(N, 0)).xyz;
            N = normalize(N);
            vec3 T = normalize(randomVec - N * dot(randomVec, N));
            vec3 B = cross(N, T);
            mat3 TBN = mat3(T, B, N);
            float radius = 8.0;
            float occlusion = 0.0;
            for (int i = 0; i < 64; ++i) {
                vec3 samplePos = TBN * u_samples[i];
                samplePos = viewPos + samplePos * radius;

                vec4 offset = u_projMat * vec4(samplePos, 1.0);
                offset = u_projMat * offset;
                offset.xyz /= offset.w;
                offset.xyz = offset.xyz * 0.5 + 0.5;
                float sampleDepth = texture(u_posTexture, offset.xy).z;
                occlusion += (sampleDepth >= samplePos.z + 0.0001 ? 1.0 : 0.0);
                float rangeCheck = smoothstep(0.0, 1.0, radius / abs(viewPos.z - sampleDepth));
                occlusion += (sampleDepth >= samplePos.z + 0.0001 ? 1.0 : 0.0) * rangeCheck;
             }
             occlusion = 1.0 - (occlusion / 64.0);
             fragColor = vec4(vec3(occlusion), 1.0);
        }
);



SSAOShaderTmp::SSAOShaderTmp() {
    this->load();

    _posTextureUniformLocation = glGetUniformLocation(_programID, "u_posTexture");
    _normalTextureUniformLocation = glGetUniformLocation(_programID, "u_normalTexture");
    _noiseTextureUniformLocation = glGetUniformLocation(_programID, "u_noiseTexture");
    _samplesUniformLocation = glGetUniformLocation(_programID, "u_samples");
    _screenSizeUniformLocation = glGetUniformLocation(_programID, "u_screenSize");
}

bool SSAOShaderTmp::load() {
    string vShader = string("#version 330 core \n") + string(vertexSSAOTmp);
    string fShader = string("#version 330 core \n") + string(fragmentSSAOTmp);

    _programID = loadProgram_tmp(reinterpret_cast<const char *>(vShader.c_str()),
                                 reinterpret_cast<const char *>(fShader.c_str()));

    assert(_programID != 0);

    return true;
}

void SSAOShaderTmp::useProgram() {
    glUseProgram(_programID);


    const int MAX_TEXTURE = 3;
    std::array<GLint, MAX_TEXTURE> uniformLocs {
            _posTextureUniformLocation, _normalTextureUniformLocation, _noiseTextureUniformLocation
    };

    for (int i = 0; i < MAX_TEXTURE; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        GLUtil::GL_ERROR_LOG();
        glUniform1i(uniformLocs[i], i);
        GLUtil::GL_ERROR_LOG();
    }
}