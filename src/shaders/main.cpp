#include <Magnum/Magnum.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GlfwApplication.h>

#include "MyShader.h"

using namespace Magnum;

class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        GL::Mesh _mesh;
        MyShader _shader;
};

MyApplication::MyApplication(const Arguments& arguments):
    Platform::Application{ arguments, Configuration{}
        .setTitle("Learn-OpenGL with Magnum Engine")
        .setWindowFlags(Configuration::WindowFlag::Resizable)
        .setSize({800, 600}),
    GLConfiguration{}.setVersion(GL::Version::GL330) } {

    struct QuadVertex {
        Vector3 position;
        Vector3 colors;
    };

    const QuadVertex vertices[]{
        { {-0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
        { { 0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
    };

    GL::Buffer vertexBuffer{ vertices };

    _mesh.setCount(Containers::arraySize(vertices))
        .addVertexBuffer(vertexBuffer, 0,
            MyShader::Position{},
            MyShader::Color{});
}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clearColor(Magnum::Color4{0.2f, 0.3f, 0.3f, 1.0f});

    // Changing green color
    //float timeValue = glfwGetTime();
    //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

    //_shader
    //    .setColor({ 0.0f, greenValue, 0.0f, 1.0f })
    //    .draw(_mesh);
    _shader.draw(_mesh);
    swapBuffers();
    redraw();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
