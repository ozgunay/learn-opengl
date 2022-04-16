#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>

class MyShader : public Magnum::GL::AbstractShaderProgram {
public:
    typedef Magnum::GL::Attribute<0, Magnum::Vector3> Position;
    typedef Magnum::GL::Attribute<1, Magnum::Vector3> Color;
    typedef Magnum::GL::Attribute<2, Magnum::Vector2> TextureCoord;

    explicit MyShader();

    MyShader& bindTexture(Magnum::GL::Texture2D& texture, Magnum::GL::Texture2D& textureSmile) {
        texture.bind(TextureUnit);
        textureSmile.bind(TextureUnitSmile);
        return *this;
    }

private:
    enum : Magnum::Int { TextureUnit = 0,
                         TextureUnitSmile };
};