#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Platform/GlfwApplication.h>

using namespace Magnum;

class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
};

MyApplication::MyApplication(const Arguments& arguments):
    Platform::Application{ arguments, Configuration{}
        .setTitle("Learn-OpenGL with Magnum Engine")
        .setWindowFlags(Configuration::WindowFlag::Resizable),
    GLConfiguration{}.setVersion(GL::Version::GL330) } {
    /* TODO: Add your initialization code here */
}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    /* TODO: Add your drawing code here */

    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
