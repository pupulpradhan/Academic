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
GLuint qvbo;
GLuint qvbo1;
GLuint lvbo;
GLuint lvbo1;
GLuint ebuffer_light;
GLuint texId;
GLuint txc;
GLuint txc_t;
GLuint txc_p;
GLuint txc_l;
GLuint frameBuffer = 0;
GLuint depthBuffer;
GLuint renderedTexture;
GLuint tri_vertexbuffer;
GLuint quad_VertexArrayID;
GLuint quad_vertexbuffer;
GLuint depthMap;
bool ctrlFlag = false;
bool altFlag = false;
int width = 1920;
int height = 1080;
unsigned int imgWidth, imgHeight;
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
cy::Vec3f centre(0.5,0.5,0.5 - 0.0000066);
cy::Vec3f lightDirV(33.0, 19.0, -35.0);
std::string rootDir;
double shadWidth = 5920;
double shadHeight = 3080;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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
    glGenTextures( 1, &depthMap );
    glBindTexture( GL_TEXTURE_2D, depthMap );
    glTexImage2D( GL_TEXTURE_2D,  0, GL_DEPTH_COMPONENT, shadWidth, shadHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
    glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0 );
    glDrawBuffer( GL_NONE );
    glReadBuffer( GL_NONE );
    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER)
           != GL_FRAMEBUFFER_COMPLETE ) return false;
    return true;
}

static void createVertexBufferForPlane(){
    // Vertex buffer for plane
    static const GLfloat normal[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };
    
    static const GLfloat g_quad_vertex_buffer_data[] = {
                -50.0f, 0.0f, -50.0f,
                 50.0f, 0.0f, -50.0f,
                -50.0f, 0.0f,  50.0f,
                -50.0f, 0.0f,  50.0f,
                 50.0f, 0.0f, -50.0f,
                 50.0f, 0.0f,  50.0f,
    };
    static const GLfloat plane_vertex_buffer_data[] = {
        0.0f, 0.0f,0.0f,
        0.0f, 1.0f,0.0f,
        1.0f, 0.0f,0.0f,
        1.0f, 0.0f,0.0f,
        0.0f, 1.0f,0.0f,
        1.0f, 1.0f,0.0f
    };
    
    glGenBuffers(1, &qvbo);
    glBindBuffer(GL_ARRAY_BUFFER, qvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    glGenBuffers(1, &qvbo1);
    glBindBuffer(GL_ARRAY_BUFFER, qvbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal), &normal, GL_STATIC_DRAW);
    glGenBuffers(1, &txc);
    glBindBuffer(GL_ARRAY_BUFFER, txc);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertex_buffer_data), plane_vertex_buffer_data, GL_STATIC_DRAW);
}

static void createVertexBufferForTeapot(cy::TriMesh *mesh){
    // Vertex buffer for Teapot
    
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
//
//    GLfloat lightVertices[] =
//    { //     COORDINATES     //
//        -0.1f, -0.1f, 0.0f,
//         0.1f, -0.1f, 0.0f,
//         0.1f, 0.1f,  0.0f,
//    };
//    glGenBuffers(1, &lvbo);
//    glBindBuffer(GL_ARRAY_BUFFER, lvbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
}
static void createVertexBufferForLight(cy::TriMesh *meshLight){
    // Vertex buffer for Teapot
    
    cy::TriMesh::TriFace faceArray[meshLight->NF()];
    cy::TriMesh::TriFace faceTextureArray[meshLight->NF()];
    cy::Vec3f vertPosArray[meshLight->NV() * 10];
    cy::Vec3f vertNormArray[meshLight->NVN() * 10];
    cy::Vec3f vertTexArray[meshLight->NV() * 10];
    
    for (int i=0; i < meshLight->NF(); i++ ){
        faceArray[i] = meshLight->F(i);
    }
    for (int i=0; i < meshLight->NF(); i++ ){
        faceTextureArray[i] = meshLight->FT(i);
    }
    for (int i=0; i < meshLight->NV(); i++ ){
        vertPosArray[i] = meshLight->V(i);
    }
    for (int i=0; i < meshLight->NVN(); i++ ){
        vertNormArray[i] = meshLight->VN(i);
    }
    for (int i=0; i < meshLight->NVT(); i++ ){
        vertTexArray[i].Set(0.0,0.0,0.0);
    }
    
    int k = meshLight->NV();
    for (int i=0; i < meshLight->NF(); i++ ){
        for (int j=0; j < 3; j++ ){
            int l = faceTextureArray[i].v[j];
            int m = faceArray[i].v[j];
            
            if(m != l){
                vertTexArray[k] = meshLight->VT(l);
                vertPosArray[k] = vertPosArray[m];
                vertNormArray[k] = vertNormArray[m];
                faceArray[i].v[j] = k;
                k++;
            }
            else{
                vertTexArray[m] = meshLight->VT(l);
            }
        }
    }
    
    glGenBuffers(1, &lvbo);
    glBindBuffer(GL_ARRAY_BUFFER, lvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*k, vertPosArray, GL_STATIC_DRAW);
    glGenBuffers(1, &lvbo1);
    glBindBuffer(GL_ARRAY_BUFFER, lvbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*k, vertNormArray, GL_STATIC_DRAW);
    glGenBuffers(1, &txc_l);
    glBindBuffer(GL_ARRAY_BUFFER, txc_l);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*k, vertTexArray, GL_STATIC_DRAW);
    glGenBuffers(1, &ebuffer_light);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer_light);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceArray[0])*meshLight->NF(), faceArray, GL_STATIC_DRAW);
}

static void updateMousePositions(GLFWwindow* window) {
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
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    cy::GLSLProgram prog;
    prog.CreateProgram();
    prog.BuildFiles("shader.vert", "shader.frag");
    if(prog.IsNull()){
        fprintf(stderr, "Error creating shader program. \n");
        return 1;
    }
    
    cy::GLSLProgram shadowProgram;
    shadowProgram.CreateProgram();
    shadowProgram.BuildFiles("shader1.vert", "shader1.frag");
    if(shadowProgram.IsNull()){
        fprintf(stderr, "Error creating shader program. \n");
        return 1;
    }
    
    cy::TriMesh mesh;
    cy::TriMesh meshLight;
    
    if(argc > 1){
        std::string ptr(args[1]);
        rootDir = ptr.substr(0, ptr.find_last_of('/')+1);
        bool success = mesh.LoadFromFileObj(args[1]);
        bool success1 = meshLight.LoadFromFileObj((rootDir + "light.obj").c_str());
        mesh.ComputeNormals();
    }
    /* initializations */
    createVertexArray();
    createVertexBufferForPlane();
    createVertexBufferForTeapot(&mesh);
    createVertexBufferForLight(&meshLight);
//    if(!createFrameBuffer()) return -1;
    
    GLint mvp_pos = glGetUniformLocation( shadowProgram.GetID(), "mvp" );
    GLint matrixShadow_pos = glGetUniformLocation( prog.GetID(), "matrixShadow" );
    GLint mvp_pos1 = glGetUniformLocation(prog.GetID(), "mvp");
    GLint mv_pos = glGetUniformLocation(prog.GetID(), "mv");
    GLint normalMatrix_pos = glGetUniformLocation(prog.GetID(), "normalMatrix");
    GLint modelMatrix_pos = glGetUniformLocation(prog.GetID(), "modelMatrix");
    GLuint pos_location = glGetAttribLocation(prog.GetID(), "pos");
    GLuint normal_location = glGetAttribLocation(prog.GetID(), "normal");
    GLint lightDir = glGetUniformLocation(prog.GetID(), "lightDir");
    GLint cameraPos = glGetUniformLocation(prog.GetID(), "cameraPos");
    GLint shininess = glGetUniformLocation(prog.GetID(), "shininess");
    GLuint pos_location2 = glGetAttribLocation(prog.GetID(), "pos");
    GLuint normal_location1 = glGetAttribLocation(prog.GetID(), "normal");
    GLint mvp_pos2 = glGetUniformLocation(prog.GetID(), "mvp");
    GLuint pos_location3 = glGetAttribLocation(prog.GetID(), "pos");
    GLint sampler = glGetUniformLocation(prog.GetID(), "shadow");
    GLuint pos_location4 = glGetAttribLocation(shadowProgram.GetID(), "pos");
    
    cy::Vec4f pos_rot;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        updateMousePositions(window);
        cy::Matrix4f rotMatrix = cy::Matrix4f::RotationXYZ(-yRot * (M_PI/180.0), -xRot * (M_PI/180.0), 0);
        cy::Matrix4f scaleMatrix = cy::Matrix4f::Scale(0.1);
        pos_rot = rotMatrix * cy::Vec4f(pos,1);
        cy::Matrix4f viewMatrix = cy::Matrix4f::View(pos_rot.XYZ() , trans.XYZ(), up.XYZ());
        cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(40.0 * (M_PI/180.0), 1920.0/1080.0, 0.1f, 1000.0f);
        cy::Matrix4f projMatrix1 = cy::Matrix4f::Perspective(70.0 * (M_PI/180.0), 1920.0/1080.0, 0.1f, 1000.0f);
        cy::Matrix4f modelMatrix = scaleMatrix;
        cy::Matrix4f transformationMatrix1 = projMatrix * viewMatrix * modelMatrix ;
        cy::Matrix4f mvMatrix = viewMatrix * scaleMatrix;
        cy::Matrix3f normalMatrix = mvMatrix.GetSubMatrix3().GetInverse().GetTranspose();
        cy::Vec3f lightTrans(-xLightRot,19.0,-yLightRot);
        cy::Vec3f lightTrans1(lightDirV.x - xLightRot,23.0,lightDirV.y - yLightRot);
        cy::Matrix4f translationLightMatrix = cy::Matrix4f::Translation(lightTrans.XYZ());
        cy::Vec4f rotLightDir = viewMatrix * translationLightMatrix * lightDirV;
        cy::Vec4f rotLightDir1 =  translationLightMatrix * lightDirV;
        cy::Matrix4f lightMatrix = cy::Matrix4f::View(rotLightDir1.XYZ() , trans.XYZ(), up.XYZ());
        cy::Matrix4f translationMatrix = projMatrix * viewMatrix * cy::Matrix4f::Scale(0.1) * cy::Matrix4f::Translation(rotLightDir1.XYZ());
        cy::Matrix4f matrixMLP = projMatrix1 * lightMatrix * cy::Matrix4f::Scale(1.0);
        cy::Matrix4f matrixShadow = cy::Matrix4f::Translation(centre) * cy::Matrix4f::Scale(0.5) * matrixMLP;
        float matrix[16];
        float matrix1[16];
        float matrix2[9];
        float matrix3[16];
        float matrix4[9];
        
        // Render the depth map
        cy::GLRenderDepth2D shadowMap;
        shadowMap.Initialize(
               true,   // depth comparison texture
               shadWidth,  // width
               shadHeight  // height
               );
        shadowMap.SetTextureFilteringMode( GL_LINEAR, GL_LINEAR );
        shadowMap.SetTextureWrappingMode( GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER );
        shadowMap.Bind();
        glClear( GL_DEPTH_BUFFER_BIT );
        glUseProgram( shadowProgram.GetID());
        matrixMLP.Get(matrix);
        glUniformMatrix4fv( mvp_pos, 1, GL_FALSE, matrix );
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(pos_location4);
        glVertexAttribPointer(pos_location4, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        shadowMap.Unbind();

        /* Teapot rendering*/
        glViewport( 0, 0, width, height );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glUseProgram(prog.GetID());
        glActiveTexture(GL_TEXTURE0);
        matrixShadow.Get(matrix);
        glUniformMatrix4fv(matrixShadow_pos, 1, GL_FALSE, matrix );
        transformationMatrix1.Get(matrix);
        glUniformMatrix4fv(mvp_pos1, 1, GL_FALSE, matrix);
        mvMatrix.Get(matrix1);
        glUniformMatrix4fv(mv_pos, 1, GL_FALSE, matrix1);
        normalMatrix.Get(matrix2);
        glUniformMatrix3fv(normalMatrix_pos, 1, GL_FALSE, matrix2);
        modelMatrix.Get(matrix4);
        glUniformMatrix3fv(modelMatrix_pos, 1, GL_FALSE, matrix4);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(pos_location);
        glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glEnableVertexAttribArray(normal_location);
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glUniform3f(lightDir, rotLightDir.x,rotLightDir.y,rotLightDir.z);
        glUniform3f(cameraPos, pos_rot.x,pos_rot.y,pos_rot.z);
        glUniform1f(shininess, 300.0);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(prog.GetID());
        glActiveTexture(GL_TEXTURE0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        /* plane */
        glBindBuffer(GL_ARRAY_BUFFER, qvbo);
        glEnableVertexAttribArray(pos_location2);
        glVertexAttribPointer(pos_location2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, qvbo1);
        glEnableVertexAttribArray(normal_location1);
        glVertexAttribPointer(normal_location1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glActiveTexture(GL_TEXTURE0);
        shadowMap.BindTexture(0);
        glUniform1i(sampler, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        /* light object*/
        glUseProgram(prog.GetID());
        translationMatrix.Get(matrix);
        glUniformMatrix4fv(mvp_pos2, 1, GL_FALSE, matrix);
        glBindBuffer(GL_ARRAY_BUFFER, lvbo);
        glEnableVertexAttribArray(pos_location3);
        glVertexAttribPointer(pos_location3, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, lvbo1);
        glEnableVertexAttribArray(normal_location1);
        glVertexAttribPointer(normal_location1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer_light);
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
