#include "Modelo.h"

#define WIDTH  600
#define HEIGHT 600

#define INTERVALO 15

int modeloSeleccionado = 1;

float posicionXEscena;
float posicionYEscena;
float posicionZEscena;

float posicionXArbol;
float posicionYArbol;
float posicionZArbol;

float posicionXLobo;
float posicionYLobo;
float posicionZLobo;

int anteriorX = 0;
int anteriorY = 0;

float anguloX = 30.0f;
float anguloY = 0.0f;

bool actualizado = false;
bool usandoMouse = false;

float zoomPorMovimientoEscena;
float zoomPorMovimientoArbol;
float zoomPorMovimientoLobo;

float desplazamientoHorizontalArbol = 0;
float desplazamientoHorizontalLobo = 0;

float desplazamientoVerticalArbol = 0;
float desplazamientoVerticalLobo = 0;

int zoomActual = 5;

Modelo modelo1;
Modelo modelo2;

void iniciar() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = {-10.0f, 10.0f, 100.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20.0, 1.0, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    modelo1.cargar("../models/Lowpoly_Tree.obj");
    modelo2.cargar("../models/Lowpoly_Fox.obj");

    posicionXEscena = modelo1.getPosicionX();
    posicionYEscena = modelo1.getPosicionY();
    posicionZEscena = modelo1.getPosicionZ() - 1.0f;

    posicionXArbol = modelo1.getPosicionX();
    posicionYArbol = modelo1.getPosicionY();
    posicionZArbol = modelo1.getPosicionZ() - 1.0f;

    posicionXLobo = modelo2.getPosicionX();
    posicionYLobo = modelo2.getPosicionY();
    posicionZLobo = modelo2.getPosicionZ() - 1.0f;

    // posicionXEscena = 0.0f;
    // posicionYEscena = -10.0f;
    // posicionZEscena = -150.0f;

    zoomPorMovimientoEscena = -modelo1.getPosicionZ() / 10.0f;
    zoomPorMovimientoArbol = -modelo1.getPosicionZ() / 10.0f;
    zoomPorMovimientoLobo = -modelo2.getPosicionZ() / 10.0f;
    // zoomPorMovimientoEscena = 150.0f / 10.0f;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    switch (modeloSeleccionado) {
        case 1: {
            glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
            // Carga modelo 1 (arbol)
            glTranslatef(posicionXEscena, posicionYEscena, posicionZEscena);
            glRotatef(anguloX, 1.0f, 0.0f, 0.0f);
            glRotatef(anguloY, 0.0f, 1.0f, 0.0f);
            modelo1.dibujar();

            // Carga modelo 2 (lobo)
            glTranslatef(20.0f, 0.0f, 0.0f);
            glScalef(0.1f, 0.1f, 0.1f);
            modelo2.dibujar();

            // Carga modelo 2 (lobo)
            glTranslatef(-200.0f, 0.0f, 200.0f);
            glRotatef(-90, 0.0f, 1.0f, 0.0f);
            modelo2.dibujar();

            // Carga modelo 2 (lobo)
            glTranslatef(-200.0f, 0.0f, 200.0f);
            glRotatef(-90, 0.0f, 1.0f, 0.0f);
            modelo2.dibujar();

            // Carga modelo 2 (lobo)
            glTranslatef(-200.0f, 0.0f, 200.0f);
            glRotatef(-90, 0.0f, 1.0f, 0.0f);
            modelo2.dibujar();

            // PARA RENDERIZAR UN CUBO
            glScalef(250.0f, 250.0f, 250.0f);
            glRotatef(90, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, -1.0f, -0.75f);
            float v[8][3] = {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
                             {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};
            float n[6][3] = {{0, 0, -1}, {0, 0, 1}, {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}};
            int f_v[6][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {0, 4, 7, 3}, {1, 5, 6, 2}, {0, 1, 5, 4}, {3, 2, 6, 7}};
            int f_n[6] = {0, 1, 2, 3, 4, 5};
            // glTranslatef(0.0f, 0.0f, -20.0f);
            // glRotatef(30.0f, 1.0f, -1.0f, 0.0f);
            for (int i = 0; i < 6; i++) {
                glBegin(GL_POLYGON);
                glNormal3fv(n[f_n[i]]);
                glVertex3fv(v[f_v[i][0]]);
                glVertex3fv(v[f_v[i][1]]);
                glVertex3fv(v[f_v[i][2]]);
                glVertex3fv(v[f_v[i][3]]);
                glEnd();
            }
            break;
        }
        case 2: {
            glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
            // Mostrar el modelo de arbol
            glTranslatef(posicionXArbol + desplazamientoHorizontalArbol, posicionYArbol, posicionZArbol + desplazamientoVerticalArbol);
            glRotatef(anguloX, 1.0f, 0.0f, 0.0f);
            glRotatef(anguloY, 0.0f, 1.0f, 0.0f);
            modelo1.dibujar();
            break;
        }
        case 3: {
            glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
            // Mostrar el modelo de lobo
            glTranslatef(posicionXLobo + desplazamientoHorizontalLobo, posicionYLobo, posicionZLobo + desplazamientoVerticalLobo);
            glRotatef(anguloX, 1.0f, 0.0f, 0.0f);
            glRotatef(anguloY, 0.0f, 1.0f, 0.0f);
            modelo2.dibujar();
            break;
        }
        default: {
            break;
        }
    }

    glutSwapBuffers();
}

void funcionTimer(int value) {
    if (actualizado) {
        actualizado = false;
        glutPostRedisplay();
    }
    glutTimerFunc(INTERVALO, funcionTimer, 0);
}

void funcionMouse(int button, int state, int x, int y) {
    actualizado = true;
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            anteriorX = x;
            anteriorY = y;
            usandoMouse = true;
        } else {
            usandoMouse = false;
        }
    } else if (state == GLUT_UP) {
        switch (button) {
            case 3: {
                if (zoomActual > 0) {
                    zoomActual--;
                    switch (modeloSeleccionado) {
                        case 1: {
                            posicionZEscena += zoomPorMovimientoEscena;
                            break;
                        }
                        case 2: {
                            posicionZArbol += zoomPorMovimientoArbol;
                            break;
                        }
                        case 3: {
                            posicionZLobo += zoomPorMovimientoLobo;
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                }
                break;
            }
            case 4: {
                if (zoomActual < 20) {
                    zoomActual++;
                    switch (modeloSeleccionado) {
                        case 1: {
                            posicionZEscena -= zoomPorMovimientoEscena;
                            break;
                        }
                        case 2: {
                            posicionZArbol -= zoomPorMovimientoArbol;
                            break;
                        }
                        case 3: {
                            posicionZLobo -= zoomPorMovimientoLobo;
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                }
                break;
            }
            default: {}
        }
    }
}

static void funcionTeclado(unsigned char key, int x, int y) {
    float rangoDesplazamiento = 1;
    switch (key) {
        case 'a': {
            if (modeloSeleccionado == 2) {
                desplazamientoHorizontalArbol -= rangoDesplazamiento;
            } else if (modeloSeleccionado == 3) {
                desplazamientoHorizontalLobo -= rangoDesplazamiento;
            }
            break;
        }
        case 'd': {
            if (modeloSeleccionado == 2) {
                desplazamientoHorizontalArbol += rangoDesplazamiento;
            } else if (modeloSeleccionado == 3) {
                desplazamientoHorizontalLobo += rangoDesplazamiento;
            }
            break;
        }
        case 'w': {
            if (modeloSeleccionado == 2) {
                desplazamientoVerticalArbol -= rangoDesplazamiento;
            } else if (modeloSeleccionado == 3) {
                desplazamientoVerticalLobo -= rangoDesplazamiento;
            }
            break;
        }
        case 's': {
            if (modeloSeleccionado == 2) {
                desplazamientoVerticalArbol += rangoDesplazamiento;
            } else if (modeloSeleccionado == 3) {
                desplazamientoVerticalLobo += rangoDesplazamiento;
            }
            break;
        }
        default: {
            break;
        }
    }
    glutPostRedisplay();
}

void funcionMotion(int x, int y) {
    if (usandoMouse) {
        actualizado = true;
        anguloY += (float)(x - anteriorX);
        anteriorX = x;
        if (anguloY > 360.0f) {
            anguloY -= 360.0f;
        } else if (anguloY < 0.0f) {
            anguloY += 360.0f;
        }
        anguloX += (float)(y - anteriorY);
        anteriorY = y;
        if (anguloX > 90.0f) {
            anguloX = 90.0f;
        } else if (anguloX < -90.0f) {
            anguloX = -90.0f;
        }
    }
}

void menuModelos(int value) {
    switch (value) {
        case 1: {
            printf("Mostrando escena general\n");
            zoomActual = 5;
            modeloSeleccionado = 1;
            break;
        }
        case 2: {
            printf("Mostrando modelo arbol\n");
            zoomActual = 5;
            desplazamientoHorizontalArbol = 0;
            modeloSeleccionado = 2;
            break;
        }
        case 3: {
            printf("Mostrando modelo lobo\n");
            zoomActual = 5;
            desplazamientoHorizontalLobo = 0;
            modeloSeleccionado = 3;
            break;
        }
        case 4: {
            exit(0);
        }
        default: {
            break;
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Modelo 3D");

    iniciar();

    int submenu1 = glutCreateMenu(menuModelos);
    glutAddMenuEntry("Escena general", 1);
    glutAddMenuEntry("Modelo arbol", 2);
    glutAddMenuEntry("Modelo lobo", 3);
    glutCreateMenu(menuModelos);
    glutAddSubMenu("Modelos", submenu1);
    glutAddMenuEntry("Salir", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutMouseFunc(funcionMouse);
    glutKeyboardFunc(funcionTeclado);
    glutMotionFunc(funcionMotion);
    glutTimerFunc(0, funcionTimer, 0);
    glutMainLoop();
    return 0;
}
