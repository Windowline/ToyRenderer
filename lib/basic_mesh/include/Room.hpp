#ifndef TOYRENDERER_ROOM_HPP
#define TOYRENDERER_ROOM_HPP

#include "BasicMeshInterface.h"
#include "Vector.hpp"

class Room : public BasicMeshInterface {
public:
    Room(float size, vec3 backColor, vec3 topColor, vec3 leftColor, vec3 rightColor, vec3 bottomColor);
    void render() const override;

private:
    unsigned int _VAO;
    unsigned int _VBO;
    unsigned int _EBO;

    float _size;
    vec3 _topColor;
    vec3 _backColor;
    vec3 _leftColor;
    vec3 _rightColor;
    vec3 _bottomColor;

    int _indSize;
};

#endif //TOYRENDERER_ROOM_HPP