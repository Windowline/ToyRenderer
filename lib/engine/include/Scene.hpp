
#ifndef Scene_hpp
#define Scene_hpp
#include <functional>
#include "Vector.hpp"
#include "Matrix.hpp"

class RenderEngine;
class Camera;
class Node;
class ShaderManager;
class FrameBufferObject;
class FullQuad;
class TexturePassShader;
class IBLPreprocessor;
class Cube;

/**
 * 장면에 필요한 렌더링 오브젝트, 빛, 그림자 정보 등을 관리하고,
 * 실질적으로 오브젝트와 셰이더, 프레임버퍼 등을 이용하여 장면을 그리는 클래스입니다.
 */
class Scene {

public:
    Scene(RenderEngine* engine, unsigned int defaultFBO);

    virtual ~Scene();

    void setScreenSize(int w, int h);

    void updateViewPosition(int dir, float delta);

    void updateViewRotation(float yaw, float pitch);

    void update();

    void render();

    std::shared_ptr<ShaderManager> shaderManager();
    std::shared_ptr<Camera> camera();

private:
    void renderDeferredPBR();
    void visitNodes(std::shared_ptr<Node> node, std::function<void(std::shared_ptr<Node>)> func);
    void buildSSAOInfo();

    //for debug
    void renderForwardPBR();
    void renderSkyBox();
    void debugIBL();
    void renderQuad(unsigned int texture, ivec2 screenSize);

    RenderEngine* _engine;
    unsigned int _defaultFBO;
    bool _rootTransformDirty;

    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Node> _rootNode;
    std::shared_ptr<Node> _plane;
    std::shared_ptr<Node> _lightSphere;//조명에 위치한 흰색 구
    std::shared_ptr<FrameBufferObject> _gBuffer;
    std::shared_ptr<FrameBufferObject> _shadowDepthBuffer;

    mat4 _shadowLightView;
    mat4 _shadowLightProj;
    mat4 _shadowLightViewProjection;

    vec3 _shadowLightPosition;
    std::vector<vec3> _lightPositions;
    std::vector<vec3> _lightColors;

    //ssao
    std::vector<vec3> _ssaoKernel;
    float _ssaoNoise[16*3];
    unsigned int _ssaoNoiseTexture;
    std::shared_ptr<FrameBufferObject> _ssaoFBO;
    std::shared_ptr<FrameBufferObject> _ssaoBlurFBO;

    std::unique_ptr<FullQuad> _fullQuad;
    std::unique_ptr<Cube> _fullCube;
    std::unique_ptr<TexturePassShader> _textureShader;
    std::unique_ptr<IBLPreprocessor> _iblPreprocessor;

};

#endif /* Scene_hpp */
