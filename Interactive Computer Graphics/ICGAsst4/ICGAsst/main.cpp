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
#include "cyGL.h"
#include "cyMatrix.h"
#include "cyVector.h"
#include "cyTriMesh.h"
#include "lodepng.h"
#include <cstring>

GLuint vao;
GLuint vbo;
GLuint vbo1;
GLuint ebuffer;
GLuint texId;
GLuint texId1;
GLuint txc;
double xRot = 0.0f;
double yRot = 0.0f;
double xLightRot = 0.0f;
double yLightRot = 0.0f;
double prevXRot = 0.0f;
double prevYRot = 0.0f;
double newXRot = 0.0f;
double newYRot = 0.0f;
double prevX = 0.0f;
double prevY = 0.0f;
double newX = 0.0f;
double newY = 0.0f;
double prevTransX = 0.0f;
double prevTransY = 0.0f;
double newTransX = 0.0f;
double newTransY = 0.0f;
double xTranslation = 0.0f;
double yTranslation = 0.0f;
bool ctrlFlag = false;
int width = 1920;
int height = 1080;
unsigned int imgWidth;
unsigned int imgHeight;

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
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS){
        ctrlFlag = true;
    }
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE){
        ctrlFlag = false;
    }
}

static void createVertexArray(){
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

static void createVertexBuffer(cy::TriMesh *mesh){
    cy::TriMesh::TriFace faceArray[mesh->NF()];
    cy::TriMesh::TriFace faceTextureArray[mesh->NF()];
    cy::Vec3f vertPosArray[mesh->NV() * 10];
    cy::Vec3f vertNormArray[mesh->NVN() * 10];
    cy::Vec3f vertTexArray[mesh->NV() * 10];
    
    for (int i=0; i < mesh->NF(); i++ ){
        faceArray[i] = mesh->F(i);
    }
    for (int i=0; i < mesh->NF(); i++ ){
        faceTextureArray[i] = mesh->FT(i);
    }
    for (int i=0; i < mesh->NV(); i++ ){
        vertPosArray[i] = mesh->V(i);
    }
    for (int i=0; i < mesh->NVN(); i++ ){
        vertNormArray[i] = mesh->VN(i);
    }
    for (int i=0; i < mesh->NVT(); i++ ){
        vertTexArray[i].Set(0.0,0.0,0.0);
    }
    
    int k = mesh->NV();
    for (int i=0; i < mesh->NF(); i++ ){
        for (int j=0; j < 3; j++ ){
        int l = faceTextureArray[i].v[j];
        int m = faceArray[i].v[j];
            
            if(m != l){
                vertTexArray[k] = mesh->VT(l);
                vertPosArray[k] = vertPosArray[m];
                vertNormArray[k] = vertNormArray[m];
                faceArray[i].v[j] = k;
                k++;
            }
            else{
                vertTexArray[m] = mesh->VT(l);
            }
        }
    }
 
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*k, vertPosArray, GL_STATIC_DRAW);
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*k, vertNormArray, GL_STATIC_DRAW);
    glGenBuffers(1, &txc);
    glBindBuffer(GL_ARRAY_BUFFER, txc);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*k, vertTexArray, GL_STATIC_DRAW);
    glGenBuffers(1, &ebuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceArray[0])*mesh->NF(), faceArray, GL_STATIC_DRAW);
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
    window = glfwCreateWindow(width, height, "My ICG Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    cy::TriMesh mesh;
    char *ptr;
    std::string rootDir;
    if(argc > 1){
        bool success = mesh.LoadFromFileObj(args[1]);
        mesh.ComputeBoundingBox();
        mesh.ComputeNormals();
        std::string ptr(args[1]);
        rootDir = ptr.substr(0, ptr.find_last_of('/')+1);
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
    cy::Vec4f lightDirV(5.0, 5.0, 5.0, 0.0);

    createVertexArray();
    createVertexBuffer(&mesh);
    //createTextureArray(&mesh);
    
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
        glEnable(GL_DEPTH_TEST);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            if (ctrlFlag){
                glfwGetCursorPos(window, &newXRot, &newYRot);
                xLightRot = xLightRot + newXRot - prevXRot;
                yLightRot = yLightRot + newYRot - prevYRot;
                prevXRot = newXRot;
                prevYRot = newYRot;
            }
            else{
                glfwGetCursorPos(window, &newX, &newY);
                xRot = xRot + newX - prevX;
                yRot = yRot + newY - prevY;
                prevX = newX;
                prevY = newY;
            }
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
        cy::Matrix4f rotMatrix = cy::Matrix4f::RotationXYZ(-yRot * (M_PI/180.0), xRot * (M_PI/180.0), 0);
        cy::Matrix4f rotLightMatrix = cy::Matrix4f::RotationXYZ(yLightRot * (M_PI/180.0) , xLightRot * (M_PI/180.0) , 0);
        cy::Matrix4f translationMatrix = cy::Matrix4f::Translation(-centre.XYZ());
        cy::Vec3f lightTrans(-newXRot,-newYRot,0);
        cy::Matrix4f translationLightMatrix = cy::Matrix4f::Translation(lightTrans.XYZ());
        
        cy::Matrix4f scaleMatrix = cy::Matrix4f::Scale(0.1);
        cy::Matrix4f viewMatrix = cy::Matrix4f::View(pos.XYZ(), trans.XYZ(), up.XYZ());
        cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(40.0 * (M_PI/180.0), 1920.0/1080.0, 0.1f, 1000.0f);
        cy::Matrix4f transMatrix = projMatrix * viewMatrix * rotMatrix * scaleMatrix * translationMatrix;
        cy::Matrix4f mvMatrix = viewMatrix * rotMatrix * scaleMatrix * translationMatrix;
        cy::Matrix3f normalMatrix = mvMatrix.GetSubMatrix3().GetInverse().GetTranspose();
        cy::Vec4f rotLightDir = viewMatrix * rotLightMatrix * translationLightMatrix * lightDirV;
        float matrix[16];
        float matrix1[16];
        float matrix2[9];
        transMatrix.Get(matrix);
        GLint mvp_pos = glGetUniformLocation(prog.GetID(), "mvp");
        glUseProgram(prog.GetID());
        glUniformMatrix4fv(mvp_pos, 1, GL_FALSE, matrix);
        mvMatrix.Get(matrix1);
        GLint mv_pos = glGetUniformLocation(prog.GetID(), "mv");
        glUseProgram(prog.GetID());
        glUniformMatrix4fv(mv_pos, 1, GL_FALSE, matrix1);
        normalMatrix.Get(matrix2);
        GLint normalMatrix_pos = glGetUniformLocation(prog.GetID(), "normalMatrix");
        glUseProgram(prog.GetID());
        glUniformMatrix3fv(normalMatrix_pos, 1, GL_FALSE, matrix2);
        GLint lightDir = glGetUniformLocation(prog.GetID(), "lightDir");
        glUseProgram(prog.GetID());
        glUniform3f(lightDir, rotLightDir.x,rotLightDir.y,rotLightDir.z);
        GLint shininess = glGetUniformLocation(prog.GetID(), "shininess");
        glUseProgram(prog.GetID());
        glUniform1f(shininess, 300.0);
        GLuint pos_location = glGetAttribLocation(prog.GetID(), "pos");
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(pos_location);
        glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE,
                                  0, (void*) 0);
        GLuint normal_location = glGetAttribLocation(prog.GetID(), "normal");
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glEnableVertexAttribArray(normal_location);
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE,
                                  0, (void*) 0);
        GLuint txc_location = glGetAttribLocation(prog.GetID(), "txc");
        glBindBuffer(GL_ARRAY_BUFFER, txc);
        glEnableVertexAttribArray(txc_location);
        glVertexAttribPointer(txc_location, 3, GL_FLOAT, GL_FALSE,
                                  0, (void*) 0);
        //glDrawArrays(GL_TRIANGLES, 0, mesh.NV());
        
        //glDisableVertexAttribArray(pos_location);
        /* tex Unit for diffuse*/
        glGenTextures(0, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        std::string filename = rootDir + std::string(mesh.M(0).map_Ks);
        
        std::vector<unsigned char> png;
        std::vector<unsigned char> image; //the raw pixels
        
        unsigned error = lodepng::load_file(png, filename.c_str()); //load the image file with given filename
        if(!error) error = lodepng::decode(image, imgWidth, imgHeight, png);
        //if there's an error, display it
        if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(
                        GL_TEXTURE_2D,
                        GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR
            );

        glTexParameteri(
                        GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR
            );

        glTexParameteri(
                        GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_S,
                        GL_REPEAT
            );

        glTexParameteri(
                        GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_T,
                        GL_REPEAT
            );
        /* BIND TEXTURE*/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);
        GLint sampler = glGetUniformLocation(prog.GetID(), "tex");
        glUseProgram(prog.GetID());
        glUniform1i(sampler, 0);
        
        /* tex Unit for specular*/
        glGenTextures(1, &texId1);
        glBindTexture(GL_TEXTURE_2D, texId1);
        std::string filename1 = rootDir + std::string(mesh.M(0).map_Kd);
        std::vector<unsigned char> png1;
        std::vector<unsigned char> image1; //the raw pixels
        
        unsigned error1 = lodepng::load_file(png1, filename1.c_str()); //load the image file with given filename
        if(!error1) error1 = lodepng::decode(image1, imgWidth, imgHeight, png1);
        //if there's an error, display it
        if(error) std::cout << "decoder error " << error1 << ": " << lodepng_error_text(error1) << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(
                        GL_TEXTURE_2D,
                        GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR
            );

        glTexParameteri(
                        GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR
            );

        glTexParameteri(
                        GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_S,
                        GL_REPEAT
            );

        glTexParameteri(
                        GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_T,
                        GL_REPEAT
            );
        /* BIND TEXTURE*/
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texId1);
        GLint sampler1 = glGetUniformLocation(prog.GetID(), "tex1");
        glUseProgram(prog.GetID());
        glUniform1i(sampler1, 1);
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

