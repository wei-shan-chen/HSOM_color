#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "matrixStack.h"
#include "shader.h"
#include "item.h"
#include "world.h"
#include "color.h"
#include "SOM.h"

#include <iostream>
#include <algorithm>
#include <array>
#include <cstdlib> 
#include <ctime> 
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

using namespace std;

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HSOM", NULL, NULL);
    if(window == NULL){
        std::cout << "failed to crerate GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<< "fialed to initialize GLAD" << std::endl;
        return -1;
    }
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    srand( time(NULL) );
    Shader ourShader("shader/vShader.vs", "shader/fShader.fs");

    create_world();
    Item square(world.square,world.squ_indices);
    Item triangle(world.tri);

    glEnable(GL_DEPTH_TEST);
    bool stop = false;
    SOM_Create();
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("hsom");   
        ImGui::Text("iter, %d", iter);
        ImGui::Text("radius, %f", neighbor);
        ImGui::Text("learning_rate, %f", n_learning_rate);
        for(int i = 0; i < num_color_type; i++){

            ImGui::TextColored(ImVec4(dataset[i].r, dataset[i].g, dataset[i].b, 255), "color");
        }
        if(ImGui::Button("STSRT"))
        {
            go = 1;
        } 
        if(ImGui::Button("STOP"))
        {
            stop = !stop;
        }               
        ImGui::End();
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

		MatrixStack model;

        if(!is_som_finished && go == 1 && !stop) {
            // std::cout << "-"<<std::endl;
            SOM_IterateOnce();
            // std::cout << "."<<std::endl;

        }
        
        double width = 1.0/static_cast<double>(map_width[level]);
        double height = 1.0/static_cast<double>(map_height[level]);
        for(int i = 0; i < map_width[level]; i++){
            for(int j = 0; j < map_height[level]; j++){
                
                //draw
                model.Push();
                model.Save(glm::translate(model.Top(), glm::vec3(((double)i)*width - 0.5f,((double)j)*height - 0.5f,0.0f)));
                model.Save(glm::scale(model.Top(),glm::vec3(width, height, 1.0f)));
                ourShader.setMat4("model", model.Top());
                ourShader.setVec3("color", glm::vec3(lattice[i][j].r,lattice[i][j].g,lattice[i][j].b));
                glBindVertexArray(square.VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                model.Pop();
            }
        }

        ImGui::ShowDemoWindow();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    SOM_Destroy();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
        if(go == 0)
            go = 1;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


