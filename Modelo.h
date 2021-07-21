#ifndef INC_3D_MODEL_LOADER_MODELO_H
#define INC_3D_MODEL_LOADER_MODELO_H

#include <cmath>
#include <ctime>
#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "Material.h"
#include "Cara.h"

#include "library/loadpng.h"
#include "library/process_image.h"
#include "library/gl_texture.h"

class Modelo {
private:
    std::vector <float*> vertices;
    std::vector <float*> texturas;
    std::vector <float*> normales;
    std::vector <Cara> caras;

    GLuint listaCaras{};

    std::string prefijo;
    std::vector <Material> materiales;
    std::map <std::string, int> mapaMateriales;

    float posicionX{};
    float posicionY{};
    float posicionZ{};

protected:
    static int contarCaracteres(std::string &str, char caracter) {
        int totalCaracteres = 0;
        int longitud = (int)str.length() - 1;
        for (int i = 0; i < longitud; i++) {
            if (str[i] == caracter) {
                totalCaracteres++;
            }
        }
        return totalCaracteres;
    };

    static bool tieneDobleSlash(std::string &str) {
        int longitud = (int)str.length() - 2;
        for (int i = 0; i < longitud; i++) {
            if (str[i] == '/' && str[i + 1] == '/') {
                return true;
            }
        }
        return false;
    };

    void cargarMaterial(const char* archivo) {
        std::vector <std::string> lineas;

        std::ifstream in(archivo);
        if (!in.is_open()) {
            printf("No se pudo cargar el material %s\n", archivo);
            return;
        }

        while (!in.eof()) {
            std::string linea;
            std::getline(in, linea);
            lineas.push_back(linea);
        }
        in.close();

        Material* mat;
        int cuentaMaterial = 0;
        char str[40];
        std::string material;
        float* a;
        float* d;
        float* s;

        for (std::string &linea : lineas) {
            if (linea[0] == 'n' && linea[1] == 'e') {
                sscanf(linea.c_str(), "newmtl %s", str);
                material = str;
                mapaMateriales[material] = cuentaMaterial;
                cuentaMaterial++;
                a = new float[4]{0.2f, 0.2f, 0.2f, 1.0f};
                d = new float[4]{0.8f, 0.8f, 0.8f, 1.0f};
                s = new float[4]{0.0f, 0.0f, 0.0f, 1.0f};
                materiales.emplace_back(Material(a, d, s));
                mat = &materiales[materiales.size() - 1];
            } else if (linea[0] == 'K') {
                switch (linea[1]) {
                    case 'a': {
                        sscanf(linea.c_str(), "Ka %f %f %f", &a[0], &a[1], &a[2]);
                        break;
                    }
                    case 'd': {
                        sscanf(linea.c_str(), "Kd %f %f %f", &d[0], &d[1], &d[2]);
                        break;
                    }
                    case 's': {
                        sscanf(linea.c_str(), "Ks %f %f %f", &s[0], &s[1], &s[2]);
                        break;
                    }
                }
            } else if (linea[0] == 'm' && linea[1] == 'a') {
                sscanf(linea.c_str(), "map_Kd %s", str);
                std::string file = prefijo + str;
                Image imagen;
                Load_Texture_Swap(&imagen, file.c_str());
                glGenTextures(1, &(mat->textura));
                glBindTexture(GL_TEXTURE_2D, mat->textura);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)imagen.w, (GLsizei)imagen.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagen.img);
                glBindTexture(GL_TEXTURE_2D, 0);
                Delete_Image(&imagen);
            }
        }
    };

    void agregarCara3v(std::string &linea) {
        int vector0;
        int vector1;
        int vector2;
        sscanf(linea.c_str(), "f %d %d %d", &vector0, &vector1, &vector2);
        int* vector = new int[3]{vector0-1, vector1-1, vector2-1};
        caras.emplace_back(Cara(3, vector, nullptr));
    }

    void agregarCara3vt(std::string &linea) {
        int vector0;
        int vector1;
        int vector2;
        int textura0;
        int textura1;
        int textura2;
        sscanf(linea.c_str(), "f %d/%d %d/%d %d/%d", &vector0, &textura0, &vector1, &textura1, &vector2, &textura2);
        int* vector = new int[3]{vector0-1, vector1-1, vector2-1};
        int* textura = new int[3]{textura0-1, textura1-1, textura2-1};
        caras.emplace_back(Cara(3, vector, textura));
    }

    void agregarCara3vn(std::string &linea) {
        int vector0;
        int vector1;
        int vector2;
        int normal;
        sscanf(linea.c_str(), "f %d//%d %d//%d %d//%d", &vector0, &normal, &vector1, &normal, &vector2, &normal);
        int* vector = new int[3]{vector0-1, vector1-1, vector2-1};
        caras.emplace_back(Cara(3, vector, nullptr, normal-1));
    }

    void agregarCara3vtn(std::string &linea) {
        int vector0;
        int vector1;
        int vector2;
        int textura0;
        int textura1;
        int textura2;
        int normal;
        sscanf(linea.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vector0, &textura0, &normal, &vector1, &textura1, &normal, &vector2, &textura2, &normal);
        int* vector = new int[3]{vector0-1, vector1-1, vector2-1};
        int* textura = new int[3]{textura0-1, textura1-1, textura2-1};
        caras.emplace_back(Cara(3, vector, textura, normal-1));
    }

    void agregarCara4v(std::string &linea) {
        int vector0;
        int vector1;
        int vector2;
        int vector3;
        sscanf(linea.c_str(), "f %d %d %d %d", &vector0, &vector1, &vector2, &vector3);
        int* vector = new int[4]{vector0-1, vector1-1, vector2-1, vector3-1};
        caras.emplace_back(Cara(4, vector, nullptr));
    }

    void agregarCara4vt(std::string &linea) {
        int vector0;
        int vector1;
        int vector2;
        int vector3;
        int textura0;
        int textura1;
        int textura2;
        int textura3;
        sscanf(linea.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &vector0, &textura0, &vector1, &textura1, &vector2, &textura2, &vector3, &textura3);
        int* vector = new int[4]{vector0-1, vector1-1, vector2-1, vector3-1};
        int* textura = new int[4]{textura0-1, textura1-1, textura2-1, textura3-1};
        caras.emplace_back(Cara(4, vector, textura));
    }

    void agregarCara4vn(std::string &linea) {
        int vector0;
        int vector1;
        int vector2;
        int vector3;
        int normal;
        sscanf(linea.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &vector0, &normal, &vector1, &normal, &vector2, &normal, &vector3, &normal);
        int* vector = new int[4]{vector0-1, vector1-1, vector2-1, vector3-1};
        caras.emplace_back(Cara(4, vector, nullptr, normal-1));
    }

    void agregarCara4vtn(std::string &linea) {
        int vector0;
        int vector1;
        int vector2;
        int vector3;
        int textura0;
        int textura1;
        int textura2;
        int textura3;
        int normal;
        sscanf(linea.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &vector0, &textura0, &normal, &vector1, &textura1, &normal, &vector2, &textura2, &normal, &vector3, &textura3, &normal);
        int* vector = new int[4]{vector0-1, vector1-1, vector2-1, vector3-1};
        int* textura = new int[4]{textura0-1, textura1-1, textura2-1, textura3-1};
        caras.emplace_back(Cara(4, vector, textura, normal-1));
    }

    void extraerPrefijo(const char* archivo) {
        std::string temporal = archivo;
        this->prefijo = "";
        int indice = (int)temporal.find_last_of('/') + 1;
        if (indice > 0) {
            this->prefijo = temporal.substr(0, indice);
        }
    }

public:
    Modelo() = default;

    void cargar(const char* archivo) {
        extraerPrefijo(archivo);

        std::string lineaLeida;
        std::vector <std::string> lineas;

        std::ifstream modeloArchivo(archivo);
        if (!modeloArchivo.is_open()) {
            printf("No se pudo cargar el archivo del modelo %s\n", archivo);
            return;
        }

        while (!modeloArchivo.eof()) {
            std::getline(modeloArchivo, lineaLeida);
            lineas.push_back(lineaLeida);
        }
        modeloArchivo.close();

        float variable1;
        float variable2;
        float variable3;

        this->posicionX = 0.0f;
        this->posicionY = 0.0f;

        float sumaX = 0.0f;
        float sumaY = 0.0f;
        float sumaZ = 0.0f;

        for (std::string &linea : lineas) {
            if (linea[0] == 'v') {
                if (linea[1] == ' ') {
                    sscanf(linea.c_str(), "v %f %f %f", &variable1, &variable2, &variable3);
                    sumaX = (variable1 > 0.0f) ? (sumaX + variable1) : (sumaX - variable1);
                    sumaY = (variable2 > 0.0f) ? (sumaY + variable2) : (sumaY - variable2);
                    sumaZ = (variable3 > 0.0f) ? (sumaZ + variable3) : (sumaZ - variable3);
                    posicionX += variable1;
                    posicionY += variable2;
                    vertices.push_back(new float[3]{variable1, variable2, variable3});
                } else if (linea[1] == 't') {
                    sscanf(linea.c_str(), "vt %f %f", &variable1, &variable2);
                    texturas.push_back(new float[2]{variable1, variable2});
                } else {
                    sscanf(linea.c_str(), "vn %f %f %f", &variable1, &variable2, &variable3);
                    normales.push_back(new float[3]{variable1, variable2, variable3});
                }
            } else if (linea[0] == 'f') {
                int cantidadAristas = contarCaracteres(linea, ' ');
                int cantidadSlash = contarCaracteres(linea, '/');
                if (cantidadSlash == 0) {
                    if (cantidadAristas == 3) {
                        agregarCara3v(linea);
                    } else {
                        agregarCara4v(linea);
                    }
                } else if (cantidadSlash == cantidadAristas) {
                    if (cantidadAristas == 3) {
                        agregarCara3vt(linea);
                    } else {
                        agregarCara4vt(linea);
                    }
                } else if (cantidadSlash == 2 * cantidadAristas) {
                    if (tieneDobleSlash(linea)) {
                        if (cantidadAristas == 3) {
                            agregarCara3vn(linea);
                        } else {
                            agregarCara4vn(linea);
                        }
                    } else {
                        if (cantidadAristas == 3) {
                            agregarCara3vtn(linea);
                        } else {
                            agregarCara4vtn(linea);
                        }
                    }
                }
            } else if (linea[0] == 'm' && linea[1] == 't') {
                char material[50];
                sscanf(linea.c_str(), "mtllib %s", &material);
                std::string rutaArchivo = prefijo + material;
                cargarMaterial(rutaArchivo.c_str());
            } else if (linea[0] == 'u' && linea[1] == 's') {
                char material[50];
                sscanf(linea.c_str(), "usemtl %s", &material);
                std::string rutaMaterial = material;
                if (mapaMateriales.find(rutaMaterial) != mapaMateriales.end()) {
                    caras.emplace_back(Cara(-1, nullptr, nullptr, mapaMateriales[rutaMaterial]));
                }
            }
        }

        bool tieneTexturas = false;
        listaCaras = glGenLists(1);
        glNewList(listaCaras, GL_COMPILE);

        for (Cara &cara : caras) {
            if (cara.numeroAristas == -1) {
                tieneTexturas = false;
                glLightfv(GL_LIGHT0, GL_AMBIENT, materiales[cara.normal].luzAmbiental);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, materiales[cara.normal].luzDifusa);
                glLightfv(GL_LIGHT0, GL_SPECULAR, materiales[cara.normal].luzEspecular);
                if (materiales[cara.normal].textura != 0) {
                    tieneTexturas = true;
                    glBindTexture(GL_TEXTURE_2D, materiales[cara.normal].textura);
                }
                continue;
            }
            if (cara.normal != -1) {
                glNormal3fv(normales[cara.normal]);
            } else {
                glDisable(GL_LIGHTING);
            }
            if (tieneTexturas) {
                glBegin(GL_POLYGON);
                for (int i = 0; i < cara.numeroAristas; i++) {
                    glTexCoord2fv(texturas[cara.texturas[i]]);
                    glVertex3fv(vertices[cara.vertices[i]]);
                }
                glEnd();
            } else {
                glBegin(GL_POLYGON);
                for (int i = 0; i < cara.numeroAristas; i++) {
                    glVertex3fv(vertices[cara.vertices[i]]);
                }
                glEnd();
            }
            if (cara.normal == -1) {
                glEnable(GL_LIGHTING);
            }
        }
        glEndList();

        printf("Archivo: %s\n", archivo);
        printf("Numero de vertices: %d\n", vertices.size());
        printf("Numero de texturas: %d\n", texturas.size());
        printf("Numero de normales: %d\n", normales.size());
        printf("Numero de caras: %d\n", caras.size());
        printf("Numero de materiales: %d\n", materiales.size());

        sumaX = sumaX / (float)vertices.size();
        sumaY = sumaY / (float)vertices.size();
        sumaZ = sumaZ / (float)vertices.size();

        posicionX = posicionX / (float)vertices.size();
        posicionX = -posicionX;

        posicionY = posicionY / (float)vertices.size();
        posicionY = -posicionY;

        posicionZ = -(float)sqrt(pow(sumaX, 2) + pow(sumaY, 2) + pow(sumaZ, 2)) * 15;

        printf("Posicion X: %f\n", posicionX);
        printf("Posicion Y: %f\n", posicionY);
        printf("Posicion Z: %f\n\n", posicionZ);

        for (Material &material : materiales) {
            delete material.luzAmbiental;
            delete material.luzDifusa;
            delete material.luzEspecular;
        }
        materiales.clear();
        mapaMateriales.clear();

        for (float* v : vertices) {
            delete v;
        }
        vertices.clear();

        for (float* t : texturas) {
            delete t;
        }
        texturas.clear();

        for (float* n : normales) {
            delete n;
        }
        normales.clear();
        caras.clear();
    };

    void dibujar() const {
        glCallList(listaCaras);
    };

    float getPosicionX() const {
        return this->posicionX;
    };

    float getPosicionY() const {
        return this->posicionY;
    };

    float getPosicionZ() const {
        return this->posicionZ;
    };
};

#endif //INC_3D_MODEL_LOADER_MODELO_H
