#ifndef TOYRENDERER_RAYTRACESHADER_HPP
#define TOYRENDERER_RAYTRACESHADER_HPP

#include "BasicShader.hpp"

class RayTraceShader : public BasicShader {
public:
    RayTraceShader();
    bool load() override;
    void useProgram() override;

    void cameraPosUniform3f(GLfloat value1, GLfloat value2, GLfloat value3) {
        assert(_cameraPosUniformLoc != -1);
        glUniform3f(_cameraPosUniformLoc, value1, value2, value3);
    }

    void cameraLocalToWorldMatUniformMatrix4fv(const GLfloat *value) {
        assert(_cameraLocalToWorldMatUniformLoc != -1);
        glUniformMatrix4fv(_cameraLocalToWorldMatUniformLoc, 1, GL_FALSE, value);
    }

    void resolutionUniform2f(GLfloat value1, GLfloat value2) {
        assert(_resolutionUnifromLoc != -1);
        glUniform2f(_resolutionUnifromLoc, value1, value2);
    }


private:
    GLint _cameraPosUniformLoc = -1;
    GLint _cameraLocalToWorldMatUniformLoc = -1;
    GLint _resolutionUnifromLoc = -1;
    GLint _textureBufferLoc = -1;
};

#endif //TOYRENDERER_RAYTRACESHADER_HPP
