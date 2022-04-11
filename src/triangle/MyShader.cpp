#include "MyShader.h"

#include <Corrade/Containers/Reference.h>
#include <Corrade/Containers/StringView.h>
#include <Corrade/Containers/StringStl.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>

using namespace Magnum;

const std::string vertexShaderSource = 
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}";

const std::string fragmantShaderSource =
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}";

MyShader::MyShader() {
    MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL330);

    //const Utility::Resource rs{ "texturedquad-data" };

    GL::Shader vert{ GL::Version::GL330, GL::Shader::Type::Vertex };
    GL::Shader frag{ GL::Version::GL330, GL::Shader::Type::Fragment };

    vert.addSource(vertexShaderSource);
    frag.addSource(fragmantShaderSource);

    CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({ vert, frag }));

    attachShaders({ vert, frag });

    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

    _colorUniform = uniformLocation("color");
    setUniform(uniformLocation("textureData"), TextureUnit);
}