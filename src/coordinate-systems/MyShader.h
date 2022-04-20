#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>

class MyShader : public Magnum::GL::AbstractShaderProgram {
public:
    typedef Magnum::GL::Attribute<0, Magnum::Vector3> Position;
    typedef Magnum::GL::Attribute<1, Magnum::Vector2> TextureCoord;

    explicit MyShader();

    MyShader& bindTexture(Magnum::GL::Texture2D& texture, Magnum::GL::Texture2D& textureSmile) {
        texture.bind(TextureUnit);
        textureSmile.bind(TextureUnitSmile);
        return *this;
    }

    void setModel(Magnum::Matrix4 model) {
        setUniform(uniformLocation("model"), model);
    }

    void setView(Magnum::Matrix4 view) {
        setUniform(uniformLocation("view"), view);
    }

    void setProjection(Magnum::Matrix4 projection) {
        setUniform(uniformLocation("projection"), projection);
    }

private:
    enum : Magnum::Int { TextureUnit = 0,
                         TextureUnitSmile };
};