#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "cpu.h"
#include <thread>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, CPU& cpu);

float deltaTime,lastFrame;
char keyPress;

// settings
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 320;

void renderChip8Display(const CPU& cpu) {
    // Set up 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCR_WIDTH, SCR_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    const float pixelWidth = (float)SCR_WIDTH / 64;
    const float pixelHeight = (float)SCR_HEIGHT / 32;
    
    glBegin(GL_QUADS);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (cpu.getPixel(x, y)) {
                // If pixel is on, draw a white square
                float x1 = x * pixelWidth;
                float y1 = y * pixelHeight;
                float x2 = (x + 1) * pixelWidth;
                float y2 = (y + 1) * pixelHeight;
                
                glColor3f(1.0f, 1.0f, 1.0f); // White color
                glVertex2f(x1, y1);
                glVertex2f(x2, y1);
                glVertex2f(x2, y2);
                glVertex2f(x1, y2);
            }
        }
    }
    glEnd();
}


int main(int argc, char **argv)
{

    if (argc != 2) {
        std::cout << "Usage: ./CHIP-8_Emulator ROMfile" << std::endl;
        return 1;
    }    
    // glfw: initialize and configure
    glfwInit();
    // Use compatibility profile instead of core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

   // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CHIP-8", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glDisable(GL_DEPTH_TEST); // Enable depth testing

    CPU cpu;
    cpu.CPUtest();
    cpu.loadFile(argv[1]);

   // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window, cpu);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        cpu.Cycle(); // call the opcode execution cycle
        renderChip8Display(cpu); // Render the CHIP-8 display

        glfwSwapBuffers(window);
        glfwPollEvents();        

        // Sleep to lock the refresh rate to 60Hz
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    glfwTerminate();
    return 0;
}



// ... (input processing and callback functions remain the same)

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, CPU &cpu)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Initialize keyPress to 0xFF (no key)
    cpu.setKeyPress(0xFF); 

    // Map GLFW keys directly to CHIP-8 keys
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) cpu.setKeyPress(0x1);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) cpu.setKeyPress(0x2);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) cpu.setKeyPress(0x3);
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) cpu.setKeyPress(0xC);
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cpu.setKeyPress(0x4);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cpu.setKeyPress(0x5);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cpu.setKeyPress(0x6);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) cpu.setKeyPress(0xD);
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cpu.setKeyPress(0x7);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cpu.setKeyPress(0x8);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cpu.setKeyPress(0x9);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) cpu.setKeyPress(0xE);
    
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) cpu.setKeyPress(0xA);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) cpu.setKeyPress(0x0);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) cpu.setKeyPress(0xB);
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) cpu.setKeyPress(0xF);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}