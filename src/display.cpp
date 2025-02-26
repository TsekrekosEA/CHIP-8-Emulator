#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "cpu.h"

#define CHIP8_INSTRUCTIONS_PER_FRAME 8

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, CPU& cpu);

char keyPress;

// Game Boy-inspired color scheme
const float BG_COLOR_R = 0.06f;    // #0f380f - dark green background
const float BG_COLOR_G = 0.22f;
const float BG_COLOR_B = 0.06f;

const float PIXEL_COLOR_R = 0.61f; // #9bbc0f - light green pixels
const float PIXEL_COLOR_G = 0.74f;
const float PIXEL_COLOR_B = 0.06f;

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
                
                glColor3f(PIXEL_COLOR_R, PIXEL_COLOR_G, PIXEL_COLOR_B); // White color
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
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

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

    glDisable(GL_DEPTH_TEST); // Disable depth testing (working in 2D)

    CPU cpu;
    cpu.loadFile(argv[1]);
    std::cout << "ROM loaded, starting emulation..." << std::endl;

   // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window, cpu);

        // render
        glClearColor(BG_COLOR_R, BG_COLOR_G, BG_COLOR_B, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < CHIP8_INSTRUCTIONS_PER_FRAME; i++) {
            cpu.Cycle(); // call opcode execution cycle, multiple times to control framerate
        }
        
        renderChip8Display(cpu); // Render the CHIP-8 display

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();        

    }

    glfwTerminate();
    return 0;
}




// Process keyboard input
void processInput(GLFWwindow *window, CPU &cpu)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Initialize keyPress to 0xFF (no key)
    cpu.setKeyPress(0xFF); 

    // CHIP-8 keypad layout:
    // 1 2 3 C    maps to    1 2 3 4
    // 4 5 6 D                Q W E R
    // 7 8 9 E                A S D F
    // A 0 B F                Z X C V

    // Row 1
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) cpu.setKeyPress(0x1);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) cpu.setKeyPress(0x2);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) cpu.setKeyPress(0x3);
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) cpu.setKeyPress(0xC);
    // Row 2  
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cpu.setKeyPress(0x4);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cpu.setKeyPress(0x5);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cpu.setKeyPress(0x6);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) cpu.setKeyPress(0xD);
    // Row 3    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cpu.setKeyPress(0x7);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cpu.setKeyPress(0x8);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cpu.setKeyPress(0x9);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) cpu.setKeyPress(0xE);
    // Row 4   
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) cpu.setKeyPress(0xA);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) cpu.setKeyPress(0x0);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) cpu.setKeyPress(0xB);
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) cpu.setKeyPress(0xF);
}

// Handle window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{glViewport(0, 0, width, height);}