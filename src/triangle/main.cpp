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


    const Vector3 vertices[]{
        {-0.5f, -0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        { 0.0f,  0.5f, 0.0f}
    };

    GL::Buffer vertexBuffer{ vertices };

    _mesh.setCount(Containers::arraySize(vertices))
        .addVertexBuffer(vertexBuffer, 0,
            MyShader::Position{});

    //// Draw a rectangle with EBO
    //const Vector3 verticesRect[] = {
    //    {0.5f,  0.5f, 0.0f},  // top right
    //    {0.5f, -0.5f, 0.0f},  // bottom right
    //    {-0.5f, -0.5f, 0.0f},  // bottom left
    //    {-0.5f,  0.5f, 0.0f}   // top left 
    //};
    //const UnsignedInt indices[]{
    //    0, 1, 3,
    //    1, 2, 3
    //};

    //GL::Buffer vertexBufferRec{ verticesRect };

    //_mesh.setCount(Containers::arraySize(indices))
    //    .addVertexBuffer(vertexBufferRec, 0,
    //        MyShader::Position{})
    //    .setIndexBuffer(GL::Buffer{ indices }, 0,
    //        GL::MeshIndexType::UnsignedInt);
    ////Wireframe mode
    //GL::Renderer::setFaceCullingMode(GL::Renderer::PolygonFacing::FrontAndBack);
    //GL::Renderer::setPolygonMode(GL::Renderer::PolygonMode::Line);
}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clearColor(Magnum::Color4{0.2f, 0.3f, 0.3f, 1.0f});

    /* TODO: Add your drawing code here */
    _shader.draw(_mesh);

    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
