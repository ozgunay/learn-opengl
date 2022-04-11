#include <Magnum/Magnum.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GlfwApplication.h>

using namespace Magnum;

class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        GL::Mesh _mesh;
        Shaders::VertexColorGL3D _shader;
};

MyApplication::MyApplication(const Arguments& arguments):
    Platform::Application{ arguments, Configuration{}
        .setTitle("Learn-OpenGL with Magnum Engine")
        .setWindowFlags(Configuration::WindowFlag::Resizable)
        .setSize({800, 600}),
    GLConfiguration{}.setVersion(GL::Version::GL330) } {
    /* TODO: Add your initialization code here */
    const Vector3 vertices[]{
    {-0.5f, -0.5f, 0.0f},
    { 0.5f, -0.5f, 0.0f},
    { 0.0f,  0.5f, 0.0f}
    };
    GL::Buffer vertexBuffer{ vertices };

    _mesh.setCount(Containers::arraySize(vertices))
        .addVertexBuffer(vertexBuffer, 0,
            Shaders::VertexColorGL3D::Position{});
}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clearColor(Magnum::Color4{0.2f, 0.3f, 0.3f, 1.0f});

    /* TODO: Add your drawing code here */
    _shader.draw(_mesh);

    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
