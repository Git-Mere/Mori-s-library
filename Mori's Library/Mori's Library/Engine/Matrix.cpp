

#include "Matrix.h"
#include <cmath>

void Math::TransformationMatrix::Reset()
{
    matrix[0][0] = 1.0f;
    matrix[0][1] = 0.0f;
    matrix[0][2] = 0.0f;
    matrix[1][0] = 0.0f;
    matrix[1][1] = 1.0f;
    matrix[1][2] = 0.0f;
    matrix[2][0] = 0.0f;
    matrix[2][1] = 0.0f;
    matrix[2][2] = 1.0f;
}

Math::TransformationMatrix::TransformationMatrix()
{
    Reset();
}

Math::TransformationMatrix Math::TransformationMatrix::operator * (TransformationMatrix m) const {
    TransformationMatrix result;

    result.matrix[0][0] = matrix[0][0] * m[0][0] + matrix[0][1] * m[1][0] + matrix[0][2] * m[2][0];
    result.matrix[0][1] = matrix[0][0] * m[0][1] + matrix[0][1] * m[1][1] + matrix[0][2] * m[2][1];
    result.matrix[0][2] = matrix[0][0] * m[0][2] + matrix[0][1] * m[1][2] + matrix[0][2] * m[2][2];
    result.matrix[1][0] = matrix[1][0] * m[0][0] + matrix[1][1] * m[1][0] + matrix[1][2] * m[2][0];
    result.matrix[1][1] = matrix[1][0] * m[0][1] + matrix[1][1] * m[1][1] + matrix[1][2] * m[2][1];
    result.matrix[1][2] = matrix[1][0] * m[0][2] + matrix[1][1] * m[1][2] + matrix[1][2] * m[2][2];
    result.matrix[2][0] = matrix[2][0] * m[0][0] + matrix[2][1] * m[1][0] + matrix[2][2] * m[2][0];
    result.matrix[2][1] = matrix[2][0] * m[0][1] + matrix[2][1] * m[1][1] + matrix[2][2] * m[2][1];
    result.matrix[2][2] = matrix[2][0] * m[0][2] + matrix[2][1] * m[1][2] + matrix[2][2] * m[2][2];

    return result;
}

Math::TransformationMatrix& Math::TransformationMatrix::operator *= (Math::TransformationMatrix m) {
    (*this) = (*this) * m;
    return (*this);
}

vec2 Math::TransformationMatrix::operator*(vec2 v) const
{
    vec2 result;
    result.x = matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2];
    result.y = matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2];
    return result;
}




Math::TranslationMatrix::TranslationMatrix(ivec2 translate) {
    matrix[0][0] = 1.0f;
    matrix[0][1] = 0.0f;
    matrix[0][2] = static_cast<float>(translate.x);
    matrix[1][0] = 0.0f;
    matrix[1][1] = 1.0f;
    matrix[1][2] = static_cast<float>(translate.y);
    matrix[2][0] = 0.0f;
    matrix[2][1] = 0.0f;
    matrix[2][2] = 1.0f;
}

Math::TranslationMatrix::TranslationMatrix(vec2 translate) {
    matrix[0][0] = 1.0f;
    matrix[0][1] = 0.0f;
    matrix[0][2] = translate.x;
    matrix[1][0] = 0.0f;
    matrix[1][1] = 1.0f;
    matrix[1][2] = translate.y;
    matrix[2][0] = 0.0f;
    matrix[2][1] = 0.0f;
    matrix[2][2] = 1.0f;
}

Math::ScaleMatrix::ScaleMatrix(float scale)
{
    matrix[0][0] = 1.0f * scale;
    matrix[0][1] = 0.0f;
    matrix[0][2] = 0.0f;
    matrix[1][0] = 0.0f;
    matrix[1][1] = 1.0f * scale;
    matrix[1][2] = 0.0f;
    matrix[2][0] = 0.0f;
    matrix[2][1] = 0.0f;
    matrix[2][2] = 1.0f;
}

Math::ScaleMatrix::ScaleMatrix(vec2 scale)
{
    matrix[0][0] = 1.0f * scale.x;
    matrix[0][1] = 0.0f;
    matrix[0][2] = 0.0f;
    matrix[1][0] = 0.0f;
    matrix[1][1] = 1.0f * scale.y;
    matrix[1][2] = 0.0f;
    matrix[2][0] = 0.0f;
    matrix[2][1] = 0.0f;
    matrix[2][2] = 1.0f;
}

Math::RotationMatrix::RotationMatrix(float theta)
{
    matrix[0][0] = cos(theta);
    matrix[0][1] = -sin(theta);
    matrix[0][2] = 0.0;
    matrix[1][0] = sin(theta);
    matrix[1][1] = cos(theta);
    matrix[1][2] = 0.0;
    matrix[2][0] = 0.0;
    matrix[2][1] = 0.0;
    matrix[2][2] = 1.0;
}
