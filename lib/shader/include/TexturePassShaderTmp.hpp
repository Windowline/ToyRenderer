#ifndef TOYRENDERER_TEXTUREPASSSHADERTMP_HPP
#define TOYRENDERER_TEXTUREPASSSHADERTMP_HPP

#include "BasicShader.hpp"

class TexturePassShaderTmp : public BasicShader {
public:
    TexturePassShaderTmp();
    bool load() override;
    void useProgram() override;

private:
    void textureUniformLocation() {
        _textureUiformLoc = glGetUniformLocation(_programID, "u_texture");
    }

    GLint _textureUiformLoc = -1;
};

#endif //TOYRENDERER_TEXTUREPASSSHADERTMP_HPP
