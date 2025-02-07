

#pragma once
#include "vec2.h"

namespace Math {
    class TransformationMatrix {
    public:
        TransformationMatrix();

        const float* operator[](int index) const { return matrix[index]; }
        TransformationMatrix operator * (TransformationMatrix m) const;
        TransformationMatrix& operator *= (TransformationMatrix m);
        vec2 operator * (vec2 v) const;
        void Reset();
    protected:
        float matrix[3][3];
    };

    class TranslationMatrix : public TransformationMatrix {
    public:
        TranslationMatrix(ivec2 translate);
        TranslationMatrix(vec2 translate);
    };

    class ScaleMatrix : public TransformationMatrix {
    public:
        ScaleMatrix(float scale);
        ScaleMatrix(vec2 scale);
    };


    class RotationMatrix : public TransformationMatrix {
    public:
        RotationMatrix(float theta);
    };


}
