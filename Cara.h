#ifndef INC_3D_MODEL_LOADER_CARA_H
#define INC_3D_MODEL_LOADER_CARA_H

class Cara {
public:
    int numeroAristas{};
    int* vertices{};
    int* texturas{};
    int normal{};

    Cara() = default;

    Cara(int numeroAristas, int* vertices, int* textura, int normal = -1) {
        this->numeroAristas = numeroAristas;
        this->vertices = vertices;
        this->texturas = textura;
        this->normal = normal;
    };
};

#endif //INC_3D_MODEL_LOADER_CARA_H
