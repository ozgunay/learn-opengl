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
    "layout (location = 0) in vec3 aPos; // the position variable has attribute position 0\n"
    "layout (location = 1) in vec3 aColor; // the color variable has attribute position 1\n"
    "out vec3 ourColor; // specify a color output to the fragment shader\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor\n"
    "ourColor = aColor; // set ourColor to the input color we got from the vertex data\n"
    "};\n";

    const std::string fragmantShaderSource =
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(ourColor, 1.0);\n"
    "}\n";

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

    _colorUniform = uniformLocation("ourColor");
    setUniform(uniformLocation("textureData"), TextureUnit);
}