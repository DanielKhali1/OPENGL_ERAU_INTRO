
#if defined(_MSC_VER)
// Make MS math.h define M_PI
#define _USE_MATH_DEFINES
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <linmath.h>
#include <SOIL2.h>
#include <cglm/cglm.h>   /* for inline */
#include <cglm/call.h>   /* for library call (this also includes cglm.h) */
#include "camera.h"

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF


#include "shader_helper.h"
mat4 perspective = GLM_MAT4_IDENTITY_INIT;
mat4 view = GLM_MAT4_IDENTITY_INIT;
mat4 model = GLM_MAT4_IDENTITY_INIT;
mat4 mvp = GLM_MAT4_IDENTITY_INIT;
int width = 800.0f, height = 600.0f;

bool lockcamerain = true;

GLuint loadTexture1(const char* texImagePath) {
    GLuint textureID = SOIL_load_OGL_texture(texImagePath,
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureID == 0) printf("could not find texture file %s", texImagePath);
    return textureID;
}
void loadTexture(const char * name) {
    GLuint bricktext = loadTexture1(name);
    glBindTexture(GL_TEXTURE_2D, bricktext);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat anisoSetting = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
    }
}

// setup VBO/VAO shit
GLuint init () {
    GLuint shaderProgram = createShader("shader.vert", "shader.frag");

   GLfloat vertices[] = {-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
                                    1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
                                    1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
                                    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
                                    -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
                                    1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f }; // base – right back};

    GLfloat texCoords[]  = {

        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
        // back colors
    };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    GLuint vbo[2];
    GLuint vao = NULL;

    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    GLuint vertexPosition = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertexPosition);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    GLuint texLoc = glGetAttribLocation(shaderProgram, "tc");
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(texLoc);

    glActiveTexture(GL_TEXTURE0);
    loadTexture("image.jpg");
    glActiveTexture(GL_TEXTURE1);   
    loadTexture("image2.jpg");
    glActiveTexture(GL_TEXTURE2);
    loadTexture("image3.jpg");
    glActiveTexture(GL_TEXTURE3);
    loadTexture("image4.jpg");

    glmc_perspective(glm_rad(height*0.1f), (float)width / (float) height, 0.1f, 100.0f, &perspective);

    srand(time(NULL));
    glUseProgram(shaderProgram);
    int i = 0;
    int j = 0;
    int k = 0;

    int index = 0;
    for ( i = 0; i < 8; i++) {
        for (j =0; j < 8; j++) {
            for (k = 0; k < 8; k++) {

                int f = i;
                GLfloat offset = (float)rand() / (float)RAND_MAX;
                GLfloat temp[] = { i*3 + offset*2 -1, j * 3+ offset * 2 - 1, k * 3 + offset * 2 - 1 };

                char buffer[20];
                char buffer2[20];

                printf("%d ", i);
                snprintf(buffer, sizeof(buffer), "offsets[%d]", index);
                glUniform3fv(glGetUniformLocation(shaderProgram, buffer), 1, temp);
                int nRandonNumber = rand() % ((3 + 1) - 0) + 0;
                snprintf(buffer2, sizeof(buffer), "n[%d]", index);

                glUniform1i(glGetUniformLocation(shaderProgram, buffer2), nRandonNumber);
                index++;
            }
        }
    }
    
    return shaderProgram;
}


void reshape(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}       

// draw
void display(GLuint shaderProgram, float deltaTime) {
    glmc_perspective(glm_rad(height * 0.1f), (float)width / (float)height, 0.1f, 100.0f, &perspective);

    move_events(deltaTime);
    updateCameraVectors();
    getViewMatrix(&view);

    glViewport(0, 0, width, height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glm_rotate(&model, 0.0011f, GLM_YUP);
    //glm_translate_z(&model, 0.001f);
    glm_mat4_mulN((mat4 * []) { &perspective, & view, & model }, 3, mvp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, mvp[0]);


    glDrawArraysInstanced(GL_TRIANGLES, 0, 18, 512);
}

void window_size_callback(GLFWwindow* window, int w, int h)
{
    width = (w);
    height =(h);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE) { if (action == GLFW_PRESS) { lockcamerain = !lockcamerain; } }


    if (!lockcamerain) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    else {

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    cameraKeyMovement(window, key, scancode, action, mods);
}

void mouse_callback(GLFWwindow* window, double xposz, double yposz) {

    float xpos = (float)(xposz);
    float ypos = (float)(yposz);
    if (lockcamerain) {

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera_mouse_callback(xoffset, yoffset);

        glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
        lastX = width / 2.0f;
        lastY = height / 2.0f;
    }
};

void window_execution() {
    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(width, height, "Hello Cube", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowAspectRatio(window, 1, 1);

    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetWindowSizeCallback(window, window_size_callback);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &width, &height);
    init_camera(window);

    glfwSetTime(0.0);
    GLuint shaderProgram = init();
    printf("%d", shaderProgram);

    float t, t_old = 0;
    float dt = glfwGetTime();
    /* Main loop */

    while (true)
    {
        glUseProgram(shaderProgram);

        /* Timing */
        t = glfwGetTime();
        dt = t - t_old;
        t_old = t;

        /* Draw one frame */
        display(shaderProgram, dt);

        /* Swap buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();

        /* Check if we are still running */
        if (glfwWindowShouldClose(window))
            break;

    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

int main(void)
{
    window_execution();
    return 0;
}
