#include <Corrade/Utility/Resource.h>
#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/StringView.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Magnum.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Timeline.h>
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

#include <iostream>

using namespace Magnum;
using namespace Math::Literals;

namespace {
    Float lastX = 400.f, lastY = 300.f;
    constexpr Float sensitivity = 0.001f;
    Float pitch{0.0f};
    Float yaw{-90.0f};
    bool firstMouse{ true };
}

class MyApplication : public Platform::Application {
public:
    explicit MyApplication(const Arguments& arguments);

private:
    void drawEvent() override;
    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
    void mouseScrollEvent(MouseScrollEvent& event) override;
    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;

    GL::Mesh _mesh;
    MyShader _shader;
    GL::Texture2D _texture;
    GL::Texture2D _smileTexture;
    std::vector<Vector3> _cubePositions;
    Vector3 _cameraPos{ 0.0f, 0.0f, 3.0f };
    Vector3 _cameraFront{ 0.0f, 0.0f, -1.0f };
    Vector3 _cameraUp{ 0.0f, 1.0f, 0.0f };
    float _deltaTime{0.0f};
    Timeline _timeline;
    Float _fov{45.0f};
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

    setCursor(Cursor::HiddenLocked);
    _timeline.start();
}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clearColor(Magnum::Color4{ 0.2f, 0.3f, 0.3f, 1.0f }).
        clear(GL::FramebufferClear::Depth);

    Matrix4 projection = Matrix4::perspectiveProjection(Math::Deg(_fov), (800.0f / 600.0f), 0.1f, 100.0f);
    _shader.setProjection(projection);
    
    Matrix4 view = Matrix4::lookAt(
        _cameraPos, _cameraPos + _cameraFront, _cameraUp).invertedRigid();

    _deltaTime = _timeline.previousFrameDuration();

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
    _timeline.nextFrame();
}

void MyApplication::mousePressEvent(MouseEvent& event) {
    if (event.button() != MouseEvent::Button::Left) return;

    event.setAccepted();
}

void MyApplication::mouseReleaseEvent(MouseEvent& event) {

    event.setAccepted();
    redraw();
}

void MyApplication::mouseMoveEvent(MouseMoveEvent& event) {

    if (firstMouse)
    {
        lastX = event.position().x();
        lastY = event.position().y();
        firstMouse = false;
    }

    float xoffset = event.position().x() - lastX;
    float yoffset = lastY - event.position().y(); // reversed since y-coordinates range from bottom to top

    lastX = event.position().x();
    lastY = event.position().y();

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    Vector3 direction{
        Math::cos(Math::Rad(yaw)) * Math::cos(Math::Rad(pitch)),
        Math::sin(Math::Rad(pitch)),
        Math::sin(Math::Rad(yaw)) * cos(Math::Rad(pitch))
    };

    _cameraFront = direction.normalized();


    redraw();
}

void MyApplication::keyPressEvent(KeyEvent& event) {
    float cameraSpeed = 10.0f * _deltaTime;
    if (event.key() == KeyEvent::Key::W) {
        _cameraPos += cameraSpeed * _cameraFront;

    }
    else if (event.key() == KeyEvent::Key::S) {
        _cameraPos -= cameraSpeed * _cameraFront;

    }
    else if (event.key() == KeyEvent::Key::A) {
        _cameraPos -= Math::cross(_cameraFront, _cameraUp).normalized() * cameraSpeed;

    }
    else if (event.key() == KeyEvent::Key::D) {
        _cameraPos += Math::cross(_cameraFront, _cameraUp).normalized() * cameraSpeed;

    }
    else return;

    event.setAccepted();
    redraw();
}

void MyApplication::mouseScrollEvent(MouseScrollEvent& event)
{
    _fov -= event.offset().y();
    if (_fov < 1.0f)
        _fov = 1.0f;
    if (_fov > 45.0f)
        _fov = 45.0f;

    std::cout << "FOV: " << _fov << "\n";
}

void MyApplication::keyReleaseEvent(KeyEvent& event) {
    /*if (event.key() == KeyEvent::Key::Up || event.key() == KeyEvent::Key::Down) {
        _mainCameraVelocity.z() = 0.0f;

    }
    else if (event.key() == KeyEvent::Key::PageDown || event.key() == KeyEvent::Key::PageUp) {
        _mainCameraVelocity.y() = 0.0f;

    }
    else if (event.key() == KeyEvent::Key::Right || event.key() == KeyEvent::Key::Left) {
        _mainCameraVelocity.x() = 0.0f;

    }
    else return;

    event.setAccepted();
    redraw();*/
}


MAGNUM_APPLICATION_MAIN(MyApplication)
