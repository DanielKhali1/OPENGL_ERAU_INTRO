
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
#include <cglm/cglm.h>   /* for inline */
#include <cglm/call.h>   /* for library call (this also includes cglm.h) */

mat4 perspective = GLM_MAT4_IDENTITY_INIT;
mat4 view = GLM_MAT4_IDENTITY_INIT;
mat4 model = GLM_MAT4_IDENTITY_INIT;
mat4 mvp = GLM_MAT4_IDENTITY_INIT;
int width = 800.0f, height = 600.0f;

void checkStatus(unsigned int shaderIndex,bool isLinking)
{

    char infoLog[512];
    int success = 1;
    glGetProgramiv(shaderIndex, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderIndex, 512, NULL, infoLog);
        printf("ERROR WITH SHADER\n");
    }
    else
    {
        printf("SUCCESS WITH SHADER\n");
    }
    if (isLinking) {
        glGetProgramiv(shaderIndex, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderIndex, 512, NULL, infoLog);
            printf("ERROR WITH SHADER\n");
        }
        else
        {
            printf("SUCCESS WITH SHADER\n");
        }
    }
    glGetError();

}

// setup VBO/VAO shit
GLuint init () {
    GLuint shaderProgram = NULL;
    const GLchar* vertex_shader =
        "#version 430 core\n"
        "in vec4 color;\n"
        "in vec3 aPos;\n"
        "uniform mat4 mvp;\n"
        "out vec4 fcolor;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = mvp * vec4(aPos, 1.0) ;\n"
        "    fcolor = color;\n"
        "}\n";

    const GLchar * fragment_shader =
        "#version 430 core\n"
        "in vec4 fcolor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = fcolor;\n"
        "}\n";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, (void * ) 0);
    glCompileShader(vertexShader);
    checkStatus(vertexShader, false);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);
    checkStatus(fragmentShader, false);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkStatus(fragmentShader, true);
    glDeleteProgram(vertexShader);
    glDeleteProgram(fragmentShader);


    GLfloat vertices[] = {
        // front
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0
    };

    GLfloat colorData[] = {
        // front colors
        1.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0,
        // back colors
        1.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    GLuint vertexPosition = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertexPosition);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    GLuint colorPosition = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(colorPosition, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(colorPosition);

    glmc_perspective(90.0f, (float)width / (float) height, 0.1f, 100.0f, &perspective);
    glm_translate_z(&view, -3.0f);
    glm_rotate(&view, -20.0f, GLM_XUP);
    glm_rotate(&view, 20.0f, GLM_ZUP);
    
    return shaderProgram;
}


void reshape(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}       

GLuint indices[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // top
    3, 2, 6,
    6, 7, 3
};
// draw
void display(GLuint shaderProgram) {

    glViewport(0, 0, width, height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm_rotate(&model, 0.01f, GLM_ZUP);
    glm_mat4_mulN((mat4 * []) { &perspective, & view, & model }, 3, mvp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, mvp[0]);


    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
}


/*======================================================================*
 * main()
 *======================================================================*/

int main(void)
{
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
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &width, &height);

    glfwSetTime(0.0);
    GLuint shaderProgram = init();
    printf("%d", shaderProgram);

    float t, t_old = 0;
    float dt = glfwGetTime();
    /* Main loop */

    while(true)
    {
        glUseProgram(shaderProgram);

        /* Timing */
        t = glfwGetTime();
        dt = t - t_old;
        t_old = t;

        /* Draw one frame */
        display(shaderProgram);

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
