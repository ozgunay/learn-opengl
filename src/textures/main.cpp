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
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Platform/GlfwApplication.h>

#include "MyShader.h"

using namespace Magnum;

class MyApplication : public Platform::Application {
public:
    explicit MyApplication(const Arguments& arguments);

private:
    void drawEvent() override;
    GL::Mesh _mesh;
    MyShader _shader;
    GL::Texture2D _texture;
    GL::Texture2D _smileTexture;
};

MyApplication::MyApplication(const Arguments& arguments) :
    Platform::Application{ arguments, Configuration{}
        .setTitle("Learn-OpenGL with Magnum Engine")
        .setWindowFlags(Configuration::WindowFlag::Resizable)
        .setSize({800, 600}),
    GLConfiguration{}.setVersion(GL::Version::GL330) } {

    struct QuadVertex {
        Vector3 position;
        Vector3 colors;
        Vector2 textureCoord;
    };

    const QuadVertex vertices[]{
        { { 0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { {-0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { {-0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } }
    };

    const UnsignedInt indices[]{
        0, 1, 3,
        1, 2, 3
    };

    PluginManager::Manager<Trade::AbstractImporter> manager;
    Containers::Pointer<Trade::AbstractImporter> importer =
        manager.loadAndInstantiate("TgaImporter");
    const Utility::Resource rs{ "textures-data" };
    if (!importer || !importer->openData(rs.getRaw("rectTexture")))
        std::exit(1);

    Containers::Optional<Trade::ImageData2D> image = importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(image);
    _texture.setWrapping(GL::SamplerWrapping::Repeat)
        .setMagnificationFilter(GL::SamplerFilter::Linear)
        .setMinificationFilter(GL::SamplerFilter::Linear, GL::SamplerMipmap::Linear)
        .setStorage(1, GL::textureFormat(image->format()), image->size())
        .setSubImage(0, {}, *image)
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

    _mesh.setCount(Containers::arraySize(indices))
        .addVertexBuffer(vertexBuffer, 0,
            MyShader::Position{},
            MyShader::Color{},
            MyShader::TextureCoord{})
        .setIndexBuffer(GL::Buffer{ indices }, 0,
            GL::MeshIndexType::UnsignedInt);
        
    _shader.bindTexture(_texture, _smileTexture);
}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clearColor(Magnum::Color4{ 0.2f, 0.3f, 0.3f, 1.0f });

    _shader.draw(_mesh);
    swapBuffers();
    redraw();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
