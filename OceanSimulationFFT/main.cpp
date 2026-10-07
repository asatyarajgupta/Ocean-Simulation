#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "loadTextures.h"
#include <iostream>
#include "skybox_data.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
float lastFrame = 0.0f;
float deltaTime = 0.0f;
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -0.1f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float pitch = 0.0f;
float yaw = -90.0f;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
Camera camera{ cameraPosition, cameraFront, cameraUp, pitch, yaw, lastX, lastY , firstMouse };

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, Camera::cameraSystem);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::vector<const char*> faces = {
        "./cubemap/px.png",
        "./cubemap/nx.png",
        "./cubemap/py.png",
        "./cubemap/ny.png",
        "./cubemap/pz.png",
        "./cubemap/nz.png"

    };
    unsigned int skybox = loadCubemap(faces);
    Shader shader{ "./vertexShader.vert", "./fragShader.frag" };
    Shader skyboxShader{ "./skyboxVertexShader.vert", "./skyboxFragShader.frag" };
    unsigned int waterTexture = loadTexture("./waterTexture.jpg", "simple");
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // mesh for oceanic surface
    const int N = 512;
    const float worldSize = 50.0f;
    std::vector<float> verts;
    std::vector<unsigned int> indices;

    for (int z = 0; z <= N; z++) {
        for (int x = 0; x <= N; x++) {
            float u = (float)x / N;
            float v = (float)z / N;
            verts.insert(verts.end(), { (u - 0.5f) * worldSize, 0.0f, (v - 0.5f) * worldSize, u, v });
        }
    }

    for (int z = 0; z < N; z++) {
        for (int x = 0; x < N; x++) {
            unsigned int i = z * (N + 1) + x;
            indices.insert(indices.end(), { i, i + 1, i + N + 1, i+1, i+N+1, i + N + 2 });
        }
    }

    unsigned int meshVAO, meshVBO, meshEBO;
    glGenVertexArrays(1, &meshVAO);
    glGenBuffers(1, &meshVBO);
    glGenBuffers(1, &meshEBO);
    glBindVertexArray(meshVAO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // quadbuffer
    unsigned int quadVAO, quadVBO;
    glCreateVertexArrays(1, &quadVAO);
    glCreateBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    camera.movementSpeed = 20.0f;
    glEnable(GL_DEPTH_TEST);

    // cubemapVBOS
    unsigned int cubemapVAO, cubemapVBO;
    glCreateVertexArrays(1, &cubemapVAO);
    glCreateBuffers(1, &cubemapVBO);
    glBindVertexArray(cubemapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // shaderNonsense
    shader.use();
    shader.setInt("material.texture_diffuse1", 0);
    shader.setInt("material.texture_specular1", 1);
    shader.setFloat("material.shininess", 0.9);

    shader.setVec3("light.position", glm::vec3(0.0f,-50.0f,0.0f));
    shader.setVec3("light.ambient", glm::vec3(0.1f));
    shader.setVec3("light.specular", glm::vec3(1.0f));
    shader.setVec3("light.diffuse", glm::vec3(1.0f));
    shader.setVec3("light.direction", glm::vec3(0.0f, -1.0f, 0.0f));
    shader.setFloat("light.constant", 1.0f);
    shader.setFloat("light.linear", 0.014f);
    shader.setFloat("light.quadratic", 0.0007f);
    shader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));



    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float time = static_cast<float>(glfwGetTime());

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,1000.0f);
        glm::mat4 view = glm::lookAt(camera.getCameraPosition(), camera.getCameraFront() + camera.getCameraPosition(), camera.getCameraUp());
        glm::mat4 skyBoxView = glm::mat4(glm::mat3(view));

        glDepthMask(GL_FALSE);
        skyboxShader.use();
        skyboxShader.setMat4("projection", projection);
        skyboxShader.setMat4("view", skyBoxView);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
        glBindVertexArray(cubemapVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        
        shader.use();
        shader.setFloat("time", time);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", camera.getCameraPosition());
        shader.setMat4("view", view);
        
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, waterTexture);

        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
        // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        for (int i = -2; i <= 2; i++) {
            for (int j = -2; j <= 2; j++) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i * worldSize, 0.0f, j * worldSize));
                shader.setMat4("model", model);
                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyBoard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyBoard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyBoard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyBoard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.processKeyBoard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.processKeyBoard(DOWN, deltaTime);
    static bool rmHeld = false;
    bool rmDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    if (rmDown && !rmHeld) {
        rmHeld = true;
        glfwSetWindowUserPointer(window, &camera);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        camera.firstMouse = true;

    }
    else if (!rmDown && rmHeld) {
        rmHeld = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }



}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
