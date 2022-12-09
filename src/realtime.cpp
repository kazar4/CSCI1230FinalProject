#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "utils/sceneparser.h"
#include "src/utils/shaderloader.h"

#include "shapes/Cone.h"
#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Sphere.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//const unsigned int SHADOW_WIDTH = m_screen_width, SHADOW_HEIGHT = 1024;


// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;
}

void Realtime::finish() {
    this->makeCurrent();
    killTimer(m_timer);

    for (PrimitiveType shape : shapes) {
        glDeleteBuffers(1, &vboMap[shape]);
        glDeleteVertexArrays(1, &vaoMap[shape]);
    }

    glDeleteProgram(m_shader);
    glDeleteProgram(m_postprocess_shader);

    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    glDeleteBuffers(1, &m_fullscreen_vbo);
    glDeleteVertexArrays(1, &m_fullscreen_vao);
    glDeleteFramebuffers(1, &m_defaultFBO);

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    m_devicePixelRatio = this->devicePixelRatio();

    // To change the value of the default framebuffer
    m_defaultFBO = 2;

    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    glClearColor(0, 0, 0, 1);

    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    m_postprocess_shader = ShaderLoader::createShaderProgram(":/resources/shaders/postprocess.vert", ":/resources/shaders/postprocess.frag");
    depth_shader = ShaderLoader::createShaderProgram(":/resources/shaders/depth.vert", ":/resources/shaders/depth.frag");

    // Set the texture.frag uniform for our texture
    glUseProgram(m_postprocess_shader);
    glUniform1i(glGetUniformLocation(m_postprocess_shader, "diffuseTexture"), 0);
    glUseProgram(0);

    glUseProgram(m_postprocess_shader);
    glUniform1i(glGetUniformLocation(m_postprocess_shader, "shadowMap"), 1);
    glUseProgram(0);

    std::vector<GLfloat> fullscreen_quad_data =
    { //     POSITIONS    //
        -1.f,  1.f, 0.0f,
         0.f, 1.f,
        -1.f, -1.f, 0.0f,
         0.f, 0.f,
         1.f, -1.f, 0.0f,
         1.f, 0.f,
         1.f,  1.f, 0.0f,
         1.f, 1.f,
        -1.f,  1.f, 0.0f,
         0.f, 1.f,
         1.f, -1.f, 0.0f,
         1.f, 0.f
    };

    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(0 * sizeof(GLfloat)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    makeFBO();
}

void Realtime::makeFBO() {
//    glGenTextures(1, &m_fbo_texture);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glBindTexture(GL_TEXTURE_2D, 0);

//    // Generate and bind a renderbuffer of the right size, set its format, then unbind
//    glGenRenderbuffers(1, &m_fbo_renderbuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
//    glBindRenderbuffer(GL_RENDERBUFFER, m_defaultFBO);

//    // Generate and bind an FBO
//    glGenFramebuffers(1, &m_fbo);
//    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

//    // Add our texture as a color attachment, and our renderbuffer as a depth+stencil attachment, to our FBO
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

//    // Unbind the FBO
//    glBindRenderbuffer(GL_RENDERBUFFER, m_defaultFBO);

    glGenFramebuffers(1, &depthMapFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthMapFBO);

    glGenTextures(1, &depthMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 m_screen_width, m_screen_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}


void Realtime::paintGL() {

    //glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    //    glClear(GL_DEPTH_BUFFER_BIT);
        //ConfigureShaderAndMatrices();

    // 2. then render scene as normal with shadow mapping (using depth map)


    //ConfigureShaderAndMatrices();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(depth_shader);
    ConfigureShaderAndMatrices();

    glViewport(0, 0, m_screen_width, m_screen_height);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    paintShapes(depth_shader);
    glUseProgram(0);

    ////////
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, m_screen_width, m_screen_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ////////

    glUseProgram(m_postprocess_shader);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glm::mat4 lightProjection = camera.getProjectionMatrix();
            //glm::perspective(glm::radians(180.f), (float)1 / (float)1, 0.1f, 100.0f);
            //camera.getProjectionMatrix();
    // for direcitonal use glm::ortho instead

    glm::mat4 lightView = glm::lookAt(glm::vec3(-6.0f, 4.0f, 4.0f),
                                      glm::vec3( 0.0f, 0.0f,  0.0f),
                                      glm::vec3(camera.up));
    // inatead ov lokAt just use rthographic
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    glUniformMatrix4fv(glGetUniformLocation(m_postprocess_shader, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
    paintShapes(m_postprocess_shader);

    //paintEffects(depthMap);


//    // Bind our framebuffer
//    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
//    glViewport(0, 0, m_fbo_width, m_fbo_height);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    paintShapes();

//    // Bind the default framebuffer
//    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
//    glViewport(0, 0, m_screen_width, m_screen_height);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    paintEffects(m_fbo_texture);


//    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//        glClear(GL_DEPTH_BUFFER_BIT);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glUseProgram(0);
}

void Realtime::ConfigureShaderAndMatrices() {
    glm::mat4 lightProjection = camera.getProjectionMatrix();

    glm::mat4 lightView = glm::lookAt(glm::vec3(-6.0f, 4.0f, 4.0f),
                                      glm::vec3( 0.0f, 0.0f,  0.0f),
                                      glm::vec3(camera.up));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    glUniformMatrix4fv(glGetUniformLocation(depth_shader, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
}


void Realtime::lightUniforms(GLuint shader) {
    const std::vector<SceneLightData>& lights = metaData.lights;

    int numLights = std::min(8, int(lights.size()));
    glUniform1i(glGetUniformLocation(shader, "numLights"), numLights);

    for (int i = 0; i < numLights; i++) {
        std::string pos = "lightPos[" + std::to_string(i) + "]";
        glUniform4fv(glGetUniformLocation(shader, pos.c_str()), 1, &lights[i].pos[0]);
        std::string color = "lightColor[" + std::to_string(i) + "]";
        glUniform4fv(glGetUniformLocation(shader, color.c_str()), 1, &lights[i].color[0]);
        std::string dir = "lightDir[" + std::to_string(i) + "]";
        glUniform4fv(glGetUniformLocation(shader, dir.c_str()), 1, &lights[i].dir[0]);
        std::string func = "lightFunc[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(shader, func.c_str()), 1, &lights[i].function[0]);
        std::string angle = "lightAngle[" + std::to_string(i) + "]";
        glUniform1f(glGetUniformLocation(shader, angle.c_str()), lights[i].angle);
        std::string penumbra = "lightPenumbra[" + std::to_string(i) + "]";
        glUniform1f(glGetUniformLocation(shader, penumbra.c_str()), lights[i].penumbra);

        std::string type = "lightType[" + std::to_string(i) + "]";
        switch (lights[i].type) {
            case (LightType::LIGHT_DIRECTIONAL):
                glUniform1i(glGetUniformLocation(shader, type.c_str()), 0);
                break;
            case (LightType::LIGHT_SPOT):
                glUniform1i(glGetUniformLocation(shader, type.c_str()), 1);
                break;
            case (LightType::LIGHT_POINT):
                glUniform1i(glGetUniformLocation(shader, type.c_str()), 2);
                break;
            default:
                break;
        }
    }
}

void Realtime::paintShapes(GLuint shader) {
    // activate the shader program
    glUseProgram(shader);

    for (RenderShapeData& shape : metaData.shapes) {
        glBindVertexArray(vaoMap[shape.primitive.type]);

        // Set uniforms for default (Phong) vertex shader
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelMat"), 1, GL_FALSE, &shape.ctm[0][0]);
        glUniformMatrix3fv(glGetUniformLocation(shader, "modelMatForNorm"), 1, GL_FALSE, &shape.inverseTranspose[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "viewMat"), 1, GL_FALSE, &m_view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMat"), 1, GL_FALSE, &m_proj[0][0]);

        SceneGlobalData& globalData = metaData.globalData;
        glUniform1f(glGetUniformLocation(shader, "k_a"), globalData.ka);
        glUniform1f(glGetUniformLocation(shader, "k_d"), globalData.kd);
        glUniform1f(glGetUniformLocation(shader, "k_s"), globalData.ks);

        glUniform4fv(glGetUniformLocation(shader, "cAmbient"), 1, &shape.primitive.material.cAmbient[0]);
        glUniform4fv(glGetUniformLocation(shader, "cDiffuse"), 1, &shape.primitive.material.cDiffuse[0]);
        glUniform4fv(glGetUniformLocation(shader, "cSpecular"), 1, &shape.primitive.material.cSpecular[0]);
        glUniform1f(glGetUniformLocation(shader, "shininess"), shape.primitive.material.shininess);

        glm::vec4 worldCameraPos = inverse(m_view) * glm::vec4(0, 0, 0, 1);
        glUniform4fv(glGetUniformLocation(shader, "camPos"), 1,  &worldCameraPos[0]);

        lightUniforms(shader);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, numVerticiesMap[shape.primitive.type]);

        // unbind
        glBindVertexArray(0);
    }
    glUseProgram(0);
}

void Realtime::paintEffects(GLuint texture) {
    glUseProgram(m_postprocess_shader);

    for (int i = 0; i < 25; i++) {
        std::string kernel = "kernel[" + std::to_string(i) + "]";
        glUniform1f(glGetUniformLocation(m_postprocess_shader, kernel.c_str()), 0.04f);
    }

    glUniform1i(glGetUniformLocation(m_postprocess_shader, "applyInvert"), settings.perPixelFilter);
    glUniform1i(glGetUniformLocation(m_postprocess_shader, "applyBlur"), settings.kernelBasedFilter);
    glUniform1i(glGetUniformLocation(m_postprocess_shader, "width"), size().width()); // TODO FIGURE OUT
    glUniform1i(glGetUniformLocation(m_postprocess_shader, "height"), size().height());

    glBindVertexArray(m_fullscreen_vao);

    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Realtime::resizeGL(int w, int h) {
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    // Regenerate the FBOs
    makeFBO();

    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    camera.w = m_screen_width;
    camera.h =  m_screen_height;
    m_proj = camera.getProjectionMatrix();
}

void Realtime::sceneChanged() {
    this->makeCurrent();

    QString iScenePath =  QString::fromStdString(settings.sceneFilePath);
    bool success = SceneParser::parse(iScenePath.toStdString(), metaData);

    if (!success) {
        std::cerr << "Error loading scene: \"" << iScenePath.toStdString() << "\"" << std::endl;
    }

    camera = Camera(metaData.cameraData, size().width(), size().height());
    m_view = camera.getViewMatrix();

    settingsChanged();
    update(); // asks for a PaintGL() call to occur
}

void Realtime::getVertexData(PrimitiveType shapeType, std::vector<float>& vertexData) {
    switch (shapeType) {
        case PrimitiveType::PRIMITIVE_CUBE:
             vertexData = Cube(settings.shapeParameter1).generateShape();
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            vertexData = Cone(settings.shapeParameter1, settings.shapeParameter2).generateShape();
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            vertexData = Cylinder(settings.shapeParameter1, settings.shapeParameter2).generateShape();
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            vertexData = Sphere(settings.shapeParameter1, settings.shapeParameter2).generateShape();
            break;
        default:
            break;
    }
}

void Realtime::settingsChanged() {
    this->makeCurrent();

    if (metaData.shapes.empty()) return;

    m_proj = camera.getProjectionMatrix();

    vboMap.clear();
    vaoMap.clear();
    numVerticiesMap.clear();

    std::vector<float> vertexData = std::vector<float>();

    for (PrimitiveType shape : shapes) {
        if (!vboMap.contains(shape)) {
            getVertexData(shape, vertexData);

            // Update number of verticies for each shape
            numVerticiesMap[shape] = vertexData.size() / 6;

            // Generate and bind VBO
            GLuint shapeVBO;
            vboMap[shape] = shapeVBO;
            glGenBuffers(1, &vboMap[shape]);
            glBindBuffer(GL_ARRAY_BUFFER, vboMap[shape]);

            // Send data to VBO
            glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

            // Generate and bind VAO
            GLuint shapeVAO;
            vaoMap[shape] = shapeVAO;
            glGenVertexArrays(1, &vaoMap[shape]);
            glBindVertexArray(vaoMap[shape]);

            // Enable and Define Attribute 0 to store Position information
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

            // Clean-up bindings
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
    }
    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

glm::mat3 Realtime::rotateMatrix(float theta, glm::vec3 axis) {
    float x = axis[0];
    float y = axis[1];
    float z = axis[2];
    return glm::mat3(cos(theta) + pow(x, 2) * (1 - cos(theta)), x * y * (1 - cos(theta)) + z * sin(theta), x * z * (1 - cos(theta)) - y * sin(theta),
                     x * y * (1 - cos(theta)) - z * sin(theta), cos(theta) + pow(y, 2) * (1 - cos(theta)), y * z * (1 - cos(theta)) + x * sin(theta),
                     x * z * (1 - cos(theta)) + y * sin(theta), y * z * (1 - cos(theta)) - x * sin(theta), cos(theta) + pow(z, 2) * (1 - cos(theta)));
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);
        float moveSpeed = .001f;

        // rotates the camera about the axis defined by the world space vector
        auto rotate = rotateMatrix(deltaX * moveSpeed, glm::vec3(0, 1, 0));
        camera.updateLook(glm::vec4(rotate * glm::vec3(camera.look), 0));

        // rotates the camera about the axis defined by a vector perpendicular to the look and up vectors of the camera
        auto rotationAxis = glm::normalize(glm::cross(glm::vec3(camera.look), glm::vec3(camera.up)));
        rotate = rotateMatrix(deltaY * moveSpeed, rotationAxis);
        camera.updateLook(glm::vec4(rotate * glm::vec3(camera.look), 0));

        m_view = camera.getViewMatrix();

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();
    float moveSpeed = deltaTime * 5.f;
    glm::vec4 moveDir;

    for (auto const& [key, isPressed] : m_keyMap) {
        if (isPressed) {
            switch (key) {
                case (Qt::Key_W):
                    moveDir = camera.look;
                    break;
                case (Qt::Key_A):
                    moveDir = glm::vec4(glm::cross(glm::vec3(camera.up), glm::vec3(camera.look)), 0);
                    break;
                case (Qt::Key_S):
                    moveDir = -camera.look;
                    break;
                case (Qt::Key_D):
                    moveDir = glm::vec4(glm::cross(glm::vec3(camera.look), glm::vec3(camera.up)), 0);
                    break;
                case (Qt::Key_Space):
                    moveDir = glm::vec4(0, 1, 0, 0);
                    break;
                case (Qt::Key_Control): // command key
                    moveDir = glm::vec4(0, -1, 0, 0);
                    break;
                default:
                    break;
            }
            camera.updatePos(camera.position + moveDir * moveSpeed);
            m_view = camera.getViewMatrix();
            break;
        }
    }
    update(); // asks for a PaintGL() call to occur
}
