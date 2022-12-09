#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#include "utils/raytracescene.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();

    void getVertexData(PrimitiveType shapeType, std::vector<float>& vertexData);
    glm::mat3 rotateMatrix(float theta, glm::vec3 axis);
    void lightUniforms(GLuint shader);

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    void makeFBO();
    void paintShapes(GLuint shader);
    void paintEffects(GLuint texture);

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;

    RenderData metaData;
    Camera camera;
    glm::mat4 m_view;
    glm::mat4 m_proj;
    std::vector<PrimitiveType> shapes = std::vector{PrimitiveType::PRIMITIVE_CUBE,
                                                    PrimitiveType::PRIMITIVE_CONE,
                                                    PrimitiveType::PRIMITIVE_SPHERE,
                                                    PrimitiveType::PRIMITIVE_CYLINDER};

    GLuint m_shader;
    GLuint m_postprocess_shader;
    GLuint depth_shader;
    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;
    GLuint m_fbo;
    GLuint m_fbo_texture;
    GLuint m_fbo_renderbuffer;
    GLuint m_defaultFBO;
    std::map<PrimitiveType, GLuint> vboMap;
    std::map<PrimitiveType, GLuint> vaoMap;
    std::map<PrimitiveType, int> numVerticiesMap;

    int m_screen_width;
    int m_screen_height;
    int m_fbo_width;
    int m_fbo_height;
    std::vector<float> blurFilter;

    GLuint depthMapFBO;
    void ConfigureShaderAndMatrices();
    GLuint depthMap;
};
