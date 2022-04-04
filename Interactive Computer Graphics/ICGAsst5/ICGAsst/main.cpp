//
//  main.cpp
//  ICGAsst
//
//  Created by Pupul Pradhan on 2/21/22.
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
GLuint txc;
GLuint txc_t;
GLuint frameBuffer = 0;
GLuint depthBuffer;
GLuint renderedTexture;
GLuint quad_vertexbuffer;
GLuint quad_VertexArrayID;
bool ctrlFlag = false;
bool altFlag = false;
int width = 1920;
int height = 1080;
unsigned int textureWidth = 640;
unsigned int textureHeight = 480;
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
double xRot_t = 0.0f;
double yRot_t = 0.0f;
double prevX_t = 0.0f;
double prevY_t = 0.0f;
double newX_t = 0.0f;
double newY_t = 0.0f;
double prevTransX_t = 0.0f;
double prevTransY_t = 0.0f;
double newTransX_t = 0.0f;
double newTransY_t = 0.0f;
double xTranslation_t = 0.0f;
double yTranslation_t = 0.0f;
cy::Vec3f trans(0.0,0.0,0.0);
cy::Vec3f up(0.0,1.0,0.0);
cy::Vec3f trans_t(0.0,0.0,0.0);
cy::Vec3f up_t(0.0,1.0,0.0);
cy::Vec3f pos(0.0,0.0,-10.0);
cy::Vec3f pos_t(0.0,0.0,-15.0);
cy::Vec3f min(0.0,0.0,0.0);
cy::Vec3f max(1.0,1.0,1.0);
cy::Vec3f centre(0.0,0.0,0.0);
cy::Vec4f lightDirV(5.0, 5.0, 5.0, 0.0);
std::string rootDir;
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void buildProgram( cy::GLSLProgram prog){
    prog.BuildFiles("shader1.vert", "shader1.frag");
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
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS){
        altFlag = true;
    }
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE){
        altFlag = false;
    }
}
static void createVertexArray(){
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

static bool createFrameBuffer(){
    
    glGenFramebuffers( 1, &frameBuffer );
    glGenTextures( 1, &renderedTexture );
    glBindTexture( GL_TEXTURE_2D, renderedTexture );
    glTexImage2D( GL_TEXTURE_2D,
                 0, GL_RGB, textureWidth, textureHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, 0 );
    glGenerateMipmap( renderedTexture );
    glTexParameteri( GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL_MAX_TEXTURE_MAX_ANISOTROPY );
    glGenRenderbuffers( 1, &depthBuffer );
    glBindRenderbuffer( GL_RENDERBUFFER, depthBuffer );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureWidth, textureHeight );
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer );
    glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0 );
    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers( 1, drawBuffers );
    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE ) return false;
    return true;
}

static void createVertexBufferForPlane(){
    // Vertex buffer for plane
    
    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);

    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };
    static const GLfloat plane_vertex_buffer_data[] = {
        0.0f, 0.0f,0.0f,
        0.0f, 1.0f,0.0f,
        1.0f, 0.0f,0.0f,
        1.0f, 0.0f,0.0f,
        0.0f, 1.0f,0.0f,
        1.0f, 1.0f,0.0f
    };
    
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    glGenBuffers(1, &txc);
    glBindBuffer(GL_ARRAY_BUFFER, txc);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertex_buffer_data), plane_vertex_buffer_data, GL_STATIC_DRAW);
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
    glGenBuffers(1, &txc_t);
    glBindBuffer(GL_ARRAY_BUFFER, txc_t);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*k, vertTexArray, GL_STATIC_DRAW);
    glGenBuffers(1, &ebuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceArray[0])*mesh->NF(), faceArray, GL_STATIC_DRAW);
    
    
}

static void updateMousePositions(GLFWwindow* window) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        if (altFlag){
            glfwGetCursorPos(window, &newX, &newY);
            xRot = xRot + newX - prevX;
            yRot = yRot + newY - prevY;
            prevX = newX;
            prevY = newY;
            
        }
        else if (ctrlFlag){
            glfwGetCursorPos(window, &newXRot, &newYRot);
            xLightRot = xLightRot + newXRot - prevXRot;
            yLightRot = yLightRot + newYRot - prevYRot;
            prevXRot = newXRot;
            prevYRot = newYRot;
        }
        else{
            glfwGetCursorPos(window, &newX_t, &newY_t);
            xRot_t = xRot_t + newX_t - prevX_t;
            yRot_t = yRot_t + newY_t - prevY_t;
            prevX_t = newX_t;
            prevY_t = newY_t;
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        
        glfwGetCursorPos(window, &newX, &newY);
        if (altFlag){
        prevX = newX;
        prevY = newY;
        }
        prevXRot = newX;
        prevYRot = newY;
        
        glfwGetCursorPos(window, &newX_t, &newY_t);
        prevX_t = newX_t;
        prevY_t = newY_t;
        
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        if (altFlag){
            glfwGetCursorPos(window, &newTransX, &newTransY);
            xTranslation = prevTransX - newTransX;
            prevTransX = newTransX;
            pos.z += 0.05 * xTranslation;
        }
        else{
            glfwGetCursorPos(window, &newTransX_t, &newTransY_t);
            xTranslation_t = prevTransX_t - newTransX_t;
            prevTransX_t = newTransX_t;
            pos_t.z += 0.05 * xTranslation_t;
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
        glfwGetCursorPos(window, &newTransX, &newTransY);
        prevTransX = newTransX;
        glfwGetCursorPos(window, &newTransX_t, &newTransY_t);
        prevTransX_t = newTransX_t;
    }
}
static void  setTextureData(cy::TriMesh *mesh){
    std::string filename = rootDir + std::string(mesh->M(0).map_Kd);
    unsigned int imgWidth, imgHeight;
    std::vector<unsigned char> png;
    std::vector<unsigned char> image; //the raw pixels

    unsigned error = lodepng::load_file(png, filename.c_str()); //load the image file with given filename
    if(!error) error = lodepng::decode(image, imgWidth, imgHeight, png);
    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

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
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL_MAX_TEXTURE_MAX_ANISOTROPY );

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
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    glfwSetKeyCallback(window, key_callback);
    
    cy::GLSLProgram prog;
    prog.CreateProgram();
    if(prog.IsNull()){
        fprintf(stderr, "Error creating shader program. \n");
        return 1;
    }
    cy::TriMesh mesh;
    char *ptr;
    
    if(argc > 1){
        bool success = mesh.LoadFromFileObj(args[1]);
        mesh.ComputeBoundingBox();
        mesh.ComputeNormals();
        std::string ptr(args[1]);
        rootDir = ptr.substr(0, ptr.find_last_of('/')+1);
    }
    createVertexArray();
    createVertexBuffer(&mesh);
    if(!createFrameBuffer()) return -1;
    createVertexBufferForPlane();
    setTextureData(&mesh);
    
    min = mesh.GetBoundMin();
    max = mesh.GetBoundMax();
    centre.x =  (min.x + max.x) / 2.0;
    centre.y =  (min.y + max.y) / 2.0;
    centre.z =  (min.z + max.z) / 2.0;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        prog.BuildFiles("shader.vert", "shader.frag");
        prog.Bind();
        glUseProgram(prog.GetID());
        updateMousePositions(window);
        cy::Matrix4f rotLightMatrix = cy::Matrix4f::RotationXYZ(yLightRot * (M_PI/180.0) , xLightRot * (M_PI/180.0) , 0);
        cy::Matrix4f rotMatrix_t = cy::Matrix4f::RotationXYZ(-yRot_t * (M_PI/180.0), -xRot_t * (M_PI/180.0), 0);
        cy::Matrix4f translationMatrix_t = cy::Matrix4f::Translation(-centre.XYZ());
        cy::Vec3f lightTrans(-newXRot,-newYRot,0);
        cy::Matrix4f translationLightMatrix = cy::Matrix4f::Translation(lightTrans.XYZ());
        cy::Matrix4f scaleMatrix_t = cy::Matrix4f::Scale(0.1);
        cy::Matrix4f viewMatrix_t = cy::Matrix4f::View(pos_t.XYZ(), trans_t.XYZ(), up_t.XYZ());
        cy::Matrix4f projMatrix_t = cy::Matrix4f::Perspective(10.0 * (M_PI/180.0), 640.0/480.0, 0.1f, 500.0f);
        cy::Matrix4f transMatrix_t = projMatrix_t * viewMatrix_t * rotMatrix_t * scaleMatrix_t * translationMatrix_t;
        cy::Matrix4f mvMatrix_t = viewMatrix_t * rotMatrix_t * scaleMatrix_t * translationMatrix_t;
        cy::Matrix3f normalMatrix_t = mvMatrix_t.GetSubMatrix3().GetInverse().GetTranspose();
        cy::Vec4f rotLightDir = viewMatrix_t * rotLightMatrix * translationLightMatrix * lightDirV;
        
        /* To render teapot */
        
        float matrix_t[16];
        float matrix1[16];
        float matrix2[9];
        transMatrix_t.Get(matrix_t);
        GLint mvp_pos_t = glGetUniformLocation(prog.GetID(), "mvp");
        glUniformMatrix4fv(mvp_pos_t, 1, GL_FALSE, matrix_t);
        mvMatrix_t.Get(matrix1);
        GLint mv_pos = glGetUniformLocation(prog.GetID(), "mv");
        glUniformMatrix4fv(mv_pos, 1, GL_FALSE, matrix1);
        normalMatrix_t.Get(matrix2);
        GLint normalMatrix_pos = glGetUniformLocation(prog.GetID(), "normalMatrix");
        glUniformMatrix3fv(normalMatrix_pos, 1, GL_FALSE, matrix2);
        GLint lightDir = glGetUniformLocation(prog.GetID(), "lightDir");
        glUniform3f(lightDir, rotLightDir.x,rotLightDir.y,rotLightDir.z);
        GLint shininess = glGetUniformLocation(prog.GetID(), "shininess");
        glUniform1f(shininess, 300.0);
        GLuint pos_location_t = glGetAttribLocation(prog.GetID(), "pos");
        glEnableVertexAttribArray(pos_location_t);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(pos_location_t, 3, GL_FLOAT, GL_FALSE,
                              0, (void*) 0);
        GLuint normal_location = glGetAttribLocation(prog.GetID(), "normal");
        glEnableVertexAttribArray(normal_location);
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE,
                              0, (void*) 0);
        GLuint txc_location_t = glGetAttribLocation(prog.GetID(), "txc");
        glEnableVertexAttribArray(txc_location_t);
        glBindBuffer(GL_ARRAY_BUFFER, txc_t);
        glVertexAttribPointer(txc_location_t, 3, GL_FLOAT, GL_FALSE,
                              0, (void*) 0);
        
        /* BIND TEXTURE*/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);
        GLint sampler_t = glGetUniformLocation(prog.GetID(), "tex");
        glUniform1i(sampler_t, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
        glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
        glViewport( 0, 0, textureWidth, textureHeight );
        glClearColor(0.5,0.5,0.5,1);
        glClear(  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        glGenerateMipmap( renderedTexture );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL_MAX_TEXTURE_MAX_ANISOTROPY );
        
        /* ******************************************************************************************************** */
        /* Render plane */
        prog.BuildFiles("shader1.vert", "shader1.frag");
        prog.Bind();
        glUseProgram(prog.GetID());
        
        // Set frame buffer target to the back buffer
        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
        glViewport( 0, 0, width, height );
        glClearColor(0, 0, 0, 1);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        cy::Vec3f planeCentre(0.5,0.3,0.0);
        cy::Matrix4f modelMatrix = cy::Matrix4f::RotationXYZ(-yRot * (M_PI/180.0), xRot * (M_PI/180.0), 0);
        cy::Matrix4f translationMatrix = cy::Matrix4f::Translation(-planeCentre.XYZ());
        cy::Matrix4f viewMatrix = cy::Matrix4f::View(pos.XYZ(), trans.XYZ(), up.XYZ());
        cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(20.0 * (M_PI/180.0), 1920.0/1080.0, 0.1f, 1000.0f);
        cy::Matrix4f transMatrix = projMatrix * viewMatrix * modelMatrix * translationMatrix;
        float matrix[16];
        transMatrix.Get(matrix);
        GLint mvp_pos = glGetUniformLocation(prog.GetID(), "mvp");
        glUniformMatrix4fv(mvp_pos, 1, GL_FALSE, matrix);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, txc);
        GLuint pos_location = glGetAttribLocation(prog.GetID(), "pos");
        glEnableVertexAttribArray(pos_location);
        glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE,
                              0, (void*) 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        GLuint txc_location = glGetAttribLocation(prog.GetID(), "txc");
        glEnableVertexAttribArray(txc_location);
        glVertexAttribPointer(txc_location, 3, GL_FLOAT, GL_FALSE,
                              0, (void*) 0);
        GLint sampler = glGetUniformLocation(prog.GetID(), "tex");
        glUniform1i(sampler, 0);
        glViewport( 500, 300, width, height );
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDisableVertexAttribArray(pos_location);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
