#ifndef INC_3D_MODEL_LOADER_MATERIAL_H
#define INC_3D_MODEL_LOADER_MATERIAL_H

#include <GL/freeglut.h>
#include <GL/glext.h>

class Material {
public:
    float* luzAmbiental{};
    float* luzDifusa{};
    float* luzEspecular{};
    GLuint textura{};

public:
    Material() = default;

    Material(float* luzAmbiental, float* luzDifusa, float* luzEspecular) {
        this->luzAmbiental = luzAmbiental;
        this->luzDifusa = luzDifusa;
        this->luzEspecular = luzEspecular;
        this->textura = 0;
    };
};

#endif //INC_3D_MODEL_LOADER_MATERIAL_H
