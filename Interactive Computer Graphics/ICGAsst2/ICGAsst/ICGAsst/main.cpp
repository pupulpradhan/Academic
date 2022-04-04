//
//  main.cpp
//  ICGAsst
//
//  Created by Pupul Pradhan on 1/30/22.
//
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "cyTriMesh.h"
#include "cyGL.h"
#include "cyMatrix.h"
#include "cyVector.h"

GLuint vao;
GLuint vbo;
double xRot = 0.0f;
double yRot = 0.0f;
double prevX = 0.0f;
double prevY = 0.0f;
double prevXRot = 0.0f;
double prevYRot = 0.0f;
double newX = 0.0f;
double newY = 0.0f;
double prevTransX = 0.0f;
double prevTransY = 0.0f;
double newTransX = 0.0f;
double newTransY = 0.0f;
double xTranslation = 0.0f;
double yTranslation = 0.0f;

static void glfwGetCursorPosition(GLFWwindow *window, float xRot, float yRot);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void buildProgram( cy::GLSLProgram prog){
    prog.BuildFiles("shader.vert", "shader.frag");
    prog.Bind();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    if (key == GLFW_KEY_F6 && action == GLFW_PRESS){
        cy::GLSLProgram prog;
        prog.CreateProgram();
        if(prog.IsNull()){
            std::cout << "Error creating shader program. \n";
        }
        buildProgram(prog);
        std::cout << "recompile done \n";
    }
}

static void createVertexArray(){
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

static void createVertexBuffer(cy::TriMesh *mesh){
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*mesh->NV(), &mesh->V(0), GL_STATIC_DRAW);
}


int main( int argc, char* args[] )
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    /* Initialize the library */
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "My ICG Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    cy::TriMesh mesh;
    if(argc > 1){
        bool success = mesh.LoadFromFileObj(args[1]);
        mesh.ComputeBoundingBox();
    }
    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    cy::Vec3f trans(0.0,0.0,0.0);
    cy::Vec3f up(0.0,1.0,0.0);
    cy::Vec3f pos(0.0,0.0,-10.0);
    cy::Vec3f min(0.0,0.0,0.0);
    cy::Vec3f max(1.0,1.0,1.0);
    cy::Vec3f centre(0.0,0.0,0.0);

    createVertexArray();
    createVertexBuffer(&mesh);
    cy::GLSLProgram prog;
    prog.CreateProgram();
    if(prog.IsNull()){
        fprintf(stderr, "Error creating shader program. \n");
        return 1;
    }
    buildProgram(prog);
    while(!mesh.IsBoundBoxReady()){
        std::cout << "Bounding box not ready \n";
    }
    min = mesh.GetBoundMin();
    max = mesh.GetBoundMax();
    
    centre.x =  (min.x + max.x) / 2.0;
    centre.y =  (min.y + max.y) / 2.0;
    centre.z =  (min.z + max.z) / 2.0;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
       
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            glfwGetCursorPos(window, &newX, &newY);
            xRot = xRot + newX - prevX;
            yRot = yRot + newY - prevY;
            prevX = newX;
            prevY = newY;
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
            glfwGetCursorPos(window, &newX, &newY);
            prevX = newX;
            prevY = newY;
            prevXRot = newX;
            prevYRot = newY;
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
            glfwGetCursorPos(window, &newTransX, &newTransY);
            xTranslation = prevTransX - newTransX;
            prevTransX = newTransX;
            pos.z += 0.05 * xTranslation;
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
            glfwGetCursorPos(window, &newTransX, &newTransY);
            prevTransX = newTransX;
        }
        cy::Matrix4f modelMatrix = cy::Matrix4f::RotationXYZ(-yRot * (M_PI/180.0), -xRot * (M_PI/180.0), 0);
        
        cy::Matrix4f translationMatrix = cy::Matrix4f::Translation(-centre.XYZ());
        cy::Matrix4f scaleMatrix = cy::Matrix4f::Scale(0.05f);
        cy::Matrix4f viewMatrix = cy::Matrix4f::View(pos.XYZ(), trans.XYZ(), up.XYZ());
        cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(40.0 * (M_PI/180.0), 1920.0/1080.0, 0.1f, 1000.0f);
        cy::Matrix4f transMatrix = projMatrix * viewMatrix * modelMatrix * scaleMatrix * translationMatrix;
        float matrix[16];
        transMatrix.Get(matrix);
        GLint mvp_pos = glGetUniformLocation(prog.GetID(), "mvp");
        glUseProgram(prog.GetID());
        glUniformMatrix4fv(mvp_pos, 1, GL_FALSE, matrix);
        GLuint pos_location = glGetAttribLocation(prog.GetID(), "pos");
        glEnableVertexAttribArray(pos_location);
        glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE,
                                  0, (void*) 0);
        glDrawArrays(GL_POINTS, 0, mesh.NV());
        glDisableVertexAttribArray(pos_location);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
