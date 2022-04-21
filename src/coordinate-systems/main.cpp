#include <Corrade/Utility/Resource.h>
#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/StringView.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Magnum.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/ImageView.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Platform/GlfwApplication.h>

#include "MyShader.h"

using namespace Magnum;
using namespace Math::Literals;

class MyApplication : public Platform::Application {
public:
    explicit MyApplication(const Arguments& arguments);

private:
    void drawEvent() override;
    GL::Mesh _mesh;
    MyShader _shader;
    GL::Texture2D _texture;
    GL::Texture2D _smileTexture;
    std::vector<Vector3> _cubePositions;
};

MyApplication::MyApplication(const Arguments& arguments) :
    Platform::Application{ arguments, Configuration{}
        .setTitle("Learn-OpenGL with Magnum Engine")
        .setWindowFlags(Configuration::WindowFlag::Resizable)
        .setSize({800, 600}),
    GLConfiguration{}.setVersion(GL::Version::GL330) } {

    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);

    struct QuadVertex {
        Vector3 position;
        Vector2 textureCoord;
    };

    const QuadVertex vertices[]{
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},

        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},

        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},

        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f},  {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}}
    };

    _cubePositions = {
        {0.0f,  0.0f,  0.0f},
        {2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        {2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        {1.3f, -2.0f, -2.5f},
        {1.5f,  2.0f, -2.5f},
        {1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5}
    };

    PluginManager::Manager<Trade::AbstractImporter> manager;
    Containers::Pointer<Trade::AbstractImporter> importer =
        manager.loadAndInstantiate("TgaImporter");
    const Utility::Resource rs{ "coordinate-systems-data" };
    if (!importer || !importer->openData(rs.getRaw("rectTexture")))
        std::exit(1);

    Containers::Optional<Trade::ImageData2D> image = importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(image);
    _texture.setWrapping(GL::SamplerWrapping::Repeat)
        .setMagnificationFilter(GL::SamplerFilter::Linear)
        .setMinificationFilter(GL::SamplerFilter::Linear, GL::SamplerMipmap::Linear)
        .setStorage(1, GL::textureFormat(image->format()), image->size())
        .setSubImage(0, {}, * image)
        .generateMipmap();

    if (!importer || !importer->openData(rs.getRaw("smileTexture")))
        std::exit(1);

    Containers::Optional<Trade::ImageData2D> smileImage = importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(smileImage);
    _smileTexture.setWrapping(GL::SamplerWrapping::Repeat)
        .setMagnificationFilter(GL::SamplerFilter::Linear)
        .setMinificationFilter(GL::SamplerFilter::Linear, GL::SamplerMipmap::Linear)
        .setStorage(1, GL::textureFormat(smileImage->format()), smileImage->size())
        .setSubImage(0, {}, * smileImage)
        .generateMipmap();

    GL::Buffer vertexBuffer{ vertices };

    _mesh.setCount(Containers::arraySize(vertices))
        .addVertexBuffer(vertexBuffer, 0,
            MyShader::Position{},
            MyShader::TextureCoord{});

    Matrix4 view = Matrix4::translation({ 0.0f, 0.0f, -4.0f });
    Matrix4 projection = Matrix4::perspectiveProjection(45.0_degf, (800.0f / 600.0f), 0.1f, 100.0f);
    
    _shader.setView(view);
    _shader.setProjection(projection);

    _shader.bindTexture(_texture, _smileTexture);

    //Vector3 cameraPos{ 0.0f, 0.0f, 3.0f };
    //Vector3 cameraTarget{ 0.0f, 0.0f, 0.0f };
    //Vector3 cameraDirection = (cameraPos - cameraTarget).normalized();
    //Vector3 up{ 0.0f, 1.0f, 0.0f };
    //Vector3 cameraRight = Math::cross(up, cameraDirection).normalized();
    //Vector3 cameraUp = Math::cross(cameraDirection, cameraRight);

}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clearColor(Magnum::Color4{ 0.2f, 0.3f, 0.3f, 1.0f }).
        clear(GL::FramebufferClear::Depth);

    const float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    
    Matrix4 view = Matrix4::lookAt(
        Vector3{ camX, 0.0, camZ },
        Vector3{ 0.0, 0.0, 0.0 },
        Vector3{ 0.0, 1.0, 0.0 }).invertedRigid();

    _shader.setView(view);

    for (auto i = 0; i < _cubePositions.size(); i++)
    {
        float angle = 20.0f * i;
        Matrix4 model = Matrix4::translation(_cubePositions[i]) * 
            Matrix4::rotation(Math::Rad(angle), Vector3{ 1.0f, 0.3f, 0.5f }.normalized());
        _shader.setModel(model);
        _shader.draw(_mesh);
    }
    swapBuffers();
    redraw();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
