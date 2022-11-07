
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
#include <SOIL2/SOIL2.h>
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
GLfloat lightPos[] = { 2, 2, 2 };

bool lockcamerain = true    ;

// setup VBO/VAO shit
GLuint init () {
    GLuint shaderProgram = createShader("shader.vert", "shader.frag");

    GLfloat normals[] = {
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 0.4158, -0.9094,
        0, 0.4158, -0.9094,
        0, 0.4158, -0.9094,
        0, 0.4158, 0.9094,
        0, 0.4158, 0.9094,
        0, 0.4158, 0.9094,
        0.9094, 0.4158, 0,
        0.9094, 0.4158, 0,
        0.9094, 0.4158, 0,
        -0.9094, 0.4158, 0,
        -0.9094, 0.4158, 0,
        -0.9094, 0.4158, 0 };

    GLfloat vertices[] = {  
        -1, 0, -1,
        1, 0, 1,
        -1, 0, 1,
        0.030688, 2.11984, 0.030688,
        -0.030688, 2.11984, -0.030688,
        -0.030688, 2.11984, 0.030688,
        -1, 0, -1,
        0.030688, 2.11984, -0.030688,
        1, 0, -1,
        1, 0, 1,
        -0.030688, 2.11984, 0.030688,
        -1, 0, 1,
        1, 0, -1,
        0.030688, 2.11984, 0.030688,
        1, 0, 1,
        -1, 0, 1,
        -0.030688, 2.11984, -0.030688,
        -1, 0, -1,
        -1, 0, -1,
        1, 0, -1,
        1, 0, 1,
        0.030688, 2.11984, 0.030688,
        0.030688, 2.11984, -0.030688,
        -0.030688, 2.11984, -0.030688,
        -1, 0, -1,
        -0.030688, 2.11984, -0.030688,
        0.030688, 2.11984, -0.030688,
        1, 0, 1,
        0.030688, 2.11984, 0.030688,
        -0.030688, 2.11984, 0.030688,
        1, 0, -1,
        0.030688, 2.11984, -0.030688,
        0.030688, 2.11984, 0.030688,
        -1, 0, 1,
        -0.030688, 2.11984, 0.030688,
        -0.030688, 2.11984, -0.030688
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    GLuint vertexPosition = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertexPosition);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    GLuint texLoc = glGetAttribLocation(shaderProgram, "aNorm");
    glVertexAttribPointer(texLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(texLoc);

    glmc_perspective(glm_rad(height*0.1f), (float)width / (float) height, 0.1f, 100.0f, &perspective);

    srand(time(NULL));
    glUseProgram(shaderProgram);
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
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 x = { 0.0, 1.0, 0.0 };
    glm_rotate(model, 0.01f, x);
    glm_mat4_mulN((mat4 * []) { &perspective, & view, & model }, 3, mvp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, mvp[0]);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, lightPos);
    glUniform3fv(glGetUniformLocation(shaderProgram, "view"), 1, cameraPos);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model[0]);



    glDrawArrays(GL_TRIANGLES, 0, 37*3);
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
