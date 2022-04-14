#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>

class MyShader : public Magnum::GL::AbstractShaderProgram {
public:
    typedef Magnum::GL::Attribute<0, Magnum::Vector3> Position;
    typedef Magnum::GL::Attribute<1, Magnum::Vector2> TextureCoordinates;

    explicit MyShader();

    MyShader& setColor(const Magnum::Color4& color) {
        setUniform(_colorUniform, color);
        return *this;
    }

    MyShader& bindTexture(Magnum::GL::Texture2D& texture) {
        texture.bind(TextureUnit);
        return *this;
    }

private:
    enum : Magnum::Int { TextureUnit = 0 };

    Magnum::Int _colorUniform;
};