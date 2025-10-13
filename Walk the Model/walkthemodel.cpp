#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// projectiles
struct Projectile {
    glm::vec3 p0;
    glm::vec3 p1;
};
std::vector<Projectile> projectiles;
float projectileLength = 5.0f;

unsigned int projectileVAO, projectileVBO;
size_t numVertices = 0;
int maxProjectiles = 10;

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// state
bool noTexture = false;
bool leftMouseButtonWasClicked = false;

int walkthemodel(string objPath1, string objPath2, string objPath3)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shader("vertexShader.vxs", "fragmentShader.frs");
    Shader crosshairShader("crosshairVertexShader.vxs", "crosshairFragmentShader.frs");
    Shader projectileShader("projectileVertexShader.vxs", "projectileFragmentShader.frs");



    // load models
    // -----------
    Model tower(objPath1);
    Model urs(objPath2);
    Model kapelle(objPath3);


    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // LET'S BUILD A CITY
    // tower
    glm::mat4 towerMtx = glm::mat4(1.0f);
    towerMtx = glm::translate(towerMtx, glm::vec3(0.0f, 0.0f, 0.0f));
    towerMtx = glm::scale(towerMtx, glm::vec3(1.0f, 1.0f, 1.0f));

    // urs
    glm::mat4 ursMtx = glm::mat4(1.0f);
    ursMtx = glm::translate(ursMtx, glm::vec3(0.0f, -5.0f, 0.0f));
    ursMtx = glm::scale(ursMtx, glm::vec3(-1.0f, -1.0f, -1.0f));

    // kapelle
    glm::mat4 kapelleMtx = glm::mat4(1.0f);
    kapelleMtx = glm::translate(kapelleMtx, glm::vec3(-15.0f, 5.0f, -30.0f));
    kapelleMtx = glm::scale(kapelleMtx, glm::vec3(1.5f, 1.5f, 1.5f));

    // PROJECTILE
    glGenVertexArrays(1, &projectileVAO);
    glGenBuffers(1, &projectileVBO);

    glBindVertexArray(projectileVAO);

    glBindBuffer(GL_ARRAY_BUFFER, projectileVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * maxProjectiles * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // CROSSHAIR
    glLineWidth(2.0f);

    float vertices[] = {
        // Horizontal line
        -10.0f,  0.0f,
         10.0f,  0.0f,
         // Vertical line
           0.0f, -10.0f,
           0.0f,  10.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // crosshair projection
    glm::mat4 projection = glm::ortho(
        -(float)SCR_WIDTH / 2.0f, (float)SCR_WIDTH / 2.0f,
        -(float)SCR_HEIGHT / 2.0f, (float)SCR_HEIGHT / 2.0f,
        -1.0f, 1.0f
    );

    crosshairShader.use();
    crosshairShader.setMat4("projection", projection);


    // setup
    float time = 1.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // enable shader before setting uniforms
        shader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render the loaded model
        shader.setMat4("model", towerMtx);
        tower.Draw(shader);

        shader.setMat4("model", ursMtx);
        urs.Draw(shader, noTexture);

        shader.setMat4("model", kapelleMtx);
        kapelle.Draw(shader);


        // crosshair
        crosshairShader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 4);


        // projectile
        projectileShader.use();
        glm::mat4 projectionProjectile = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        projectileShader.setMat4("projection", projectionProjectile);
        projectileShader.setMat4("view", view);
        projectileShader.setFloat("translate", (1.0/200.0) * time);



        glBindVertexArray(projectileVAO);
        glDrawArrays(GL_LINES, 0, (GLsizei)(projectiles.size() * 2)); // two vertices per line
        
        // reset
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        
        ++time;
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void addProjectile(const glm::vec3& p0, const glm::vec3& p1) {
    if (projectiles.size() <= maxProjectiles) {
        projectiles.push_back({ p0, p1 });

        GLfloat lineData[6] = {
            p0.x, p0.y, p0.z,
            p1.x, p1.y, p1.z
        };

        glBindBuffer(GL_ARRAY_BUFFER, projectileVBO);
        glBufferSubData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), sizeof(lineData), lineData);
        numVertices += 6; // 6 floats per line (2 vertices × 3 coords)
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        noTexture = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        leftMouseButtonWasClicked = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && leftMouseButtonWasClicked) {
        // create new projectil at current camera
        glm::vec3 start = camera.Position;
        glm::vec3 end = camera.Position + camera.Front * projectileLength;
        addProjectile(start, end);

        leftMouseButtonWasClicked = false;
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

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}