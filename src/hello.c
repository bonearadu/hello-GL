#include <stdbool.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

/* Rectangle vertices. */
const GLfloat vertices[] = {
   0.5f,  0.5f, 0.0f,  // top right
   0.5f, -0.5f, 0.0f,  // bottom right
  -0.5f, -0.5f, 0.0f,  // bottom left
  -0.5f,  0.5f, 0.0f   // top left 
};

const GLuint indices[] = {
  0, 1, 3,  // First triangle
  1, 2, 3   // Second triangle
};

const GLchar *vertexShaderSource
  = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}";

const GLchar *fragmentShaderSource
  = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

/* Callback function for window resize. */
static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window) {
  /* Close window on ESCAPE pressed. */
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

/* Checks if compilation was successful. Prints log otherwise. */
static bool compilationSuccessful(GLuint shader) {
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    GLchar infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    return false;
  }

  return true;
}

int main() {
  /* Initialize GLFW and OpenGL version. */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create GLFW window. */
  GLFWwindow *window
    = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello, OpenGL!", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  /* Check if GLAD loaded correctly. */
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    glfwTerminate();
    return -1;
  }

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  /* Initialzie VBO an buffer vertex data to GPU. */
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* Initialzie EBO an buffer vertex data to GPU. */
  GLuint EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  /* Compile vertex shader. */
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  /* Check whether compilation was successful. */
  if (!compilationSuccessful(vertexShader)) {
    glfwTerminate();
    return -1;
  }

  /* Compile fragment shader. */
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);


  /* Check whether compilation was successful. */
  if (!compilationSuccessful(fragmentShader)) {
    glfwTerminate();
    return -1;
  }

  /* Create and link shader program. */
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  /* Cleanup. */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  /* Tells OpenGL how to interpret vertex data. */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
  glEnableVertexAttribArray(0);

  /* Set viewport size and callback function for window resizes. */
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  /* Render loop. */
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draws elements buffered in VAO using vertex data from the
       corresponding VBO and indices from the EBO. Uses shaders defined
       in shaderProgram. */
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
  return 0;
}