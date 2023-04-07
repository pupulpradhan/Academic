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
GLuint ebuffer1;
GLuint texId;
GLuint texId1;
GLuint texId2;
GLuint texId3;
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
bool spaceFlag = false;
int width = 1920;
int height = 1080;
unsigned int imgWidth, imgHeight;
unsigned int textureWidth = 640;
unsigned int textureHeight = 480;
double xRot = 0.0f;
double yRot = 0.0f;
double xLightRot = -9.0f;
double yLightRot = -13.50f;
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
cy::Vec3f pos(10.0,10.0,20.0);
cy::Vec3f pos_t(0.0,3.0,-10.0);
cy::Vec3f min(0.0,0.0,0.0);
cy::Vec3f max(1.0,1.0,1.0);
cy::Vec3f centre(0.5,0.5,0.5 - 0.0000005);
//cy::Vec3f lightDirV(10.0, 10.0, -20.0);
cy::Vec3f lightDirV(-3,4.5,-7.4);
std::string rootDir;
double shadWidth = 5920;
double shadHeight = 3080;
int tLevel = 60;
float projectorHeightControl = 0;
float wavetime = 0.0;
float amplitude = 0.0;
float phase = 0.0;
float amp1 = 0.002;
float amp2 = 0.003;
float amp3 = 0.025;

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
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        spaceFlag = true;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
        spaceFlag = false;
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        if(tLevel != 0)
        tLevel--;
        if(key == GLFW_KEY_A && action == GLFW_PRESS){
            amp1--;
            amp2--;
            amp3--;
        }
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
        projectorHeightControl++;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        if(projectorHeightControl != 0.0)
        projectorHeightControl--;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        tLevel++;
        if(key == GLFW_KEY_A && action == GLFW_PRESS){
            amp1++;
            amp2++;
            amp3++;
        }
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
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    
    static const GLfloat g_quad_vertex_buffer_data[] = { // -1 to +1
        -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,

    };
    static const GLfloat plane_vertex_buffer_data[] = {
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
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

static void createVertexBufferForTriangle(){
    // Vertex buffer for CUBE
    
    static const GLfloat traingle_buffer_data[] = {
        -1.0f, -1.0f, 0.999f,
        3.0f, -1.0f, 0.999f,
        -1.0f,  3.0f, 0.999f
    };
    
    glGenBuffers(1, &tri_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tri_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(traingle_buffer_data), traingle_buffer_data, GL_STATIC_DRAW);
    
}

static void createVertexBufferForObject(cy::TriMesh *mesh){
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

static void  setNormalTextureData(std::string name){
    unsigned int imgWidth, imgHeight;
    std::string filename = name;
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

static void  setDisplacementTextureData(std::string name){
    unsigned int imgWidth, imgHeight;
    std::string filename = name;
    std::vector<unsigned char> png;
    std::vector<unsigned char> image; //the raw pixels
    
    unsigned error = lodepng::load_file(png, filename.c_str()); //load the image file with given filename
    if(!error) error = lodepng::decode(image, imgWidth, imgHeight, png);
    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    
    glGenTextures(1, &texId1);
    glBindTexture(GL_TEXTURE_2D, texId1);
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
}

static std::vector<unsigned char> loadImageToTex(std::string filename){
    std::vector<unsigned char> png;
    std::vector<unsigned char> image; //the raw pixels
    
    unsigned error = lodepng::load_file(png, filename.c_str()); //load the image file with given filename
    if(!error) error = lodepng::decode(image, imgWidth, imgHeight, png);
    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    return image;
    
}

static void  setEnvTextureData(){
    std::string filename1 = rootDir + "top.png"; //left
        std::vector<unsigned char> image1 = loadImageToTex(filename1);
        std::string filename2 = rootDir + "top.png"; //right
        std::vector<unsigned char> image2 = loadImageToTex(filename2);
        std::string filename3 = rootDir + "top.png"; //top
        std::vector<unsigned char> image3 = loadImageToTex(filename3);
        std::string filename4 = rootDir + "top.png"; // bottom
        std::vector<unsigned char> image4 = loadImageToTex(filename4);
        std::string filename5 = rootDir + "top.png"; // back
        std::vector<unsigned char> image5 = loadImageToTex(filename5);
        std::string filename6 = rootDir + "top.png"; //front
        std::vector<unsigned char> image6 = loadImageToTex(filename6);
    glGenTextures( 1, &texId2 );
    glBindTexture( GL_TEXTURE_CUBE_MAP, texId2 );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image4.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image5.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image6.data());
    glGenerateMipmap( GL_TEXTURE_CUBE_MAP );
    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR
        );
    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

}
static void  setEnvTextureData1(){
    std::string filename1 = rootDir + "right.png"; //left
        std::vector<unsigned char> image1 = loadImageToTex(filename1);
        std::string filename2 = rootDir + "left.png"; //right
        std::vector<unsigned char> image2 = loadImageToTex(filename2);
        std::string filename3 = rootDir + "top.png"; //top
        std::vector<unsigned char> image3 = loadImageToTex(filename3);
        std::string filename4 = rootDir + "bottom.png"; // bottom
        std::vector<unsigned char> image4 = loadImageToTex(filename4);
        std::string filename5 = rootDir + "back.png"; // back
        std::vector<unsigned char> image5 = loadImageToTex(filename5);
        std::string filename6 = rootDir + "front.png"; //front
        std::vector<unsigned char> image6 = loadImageToTex(filename6);
    glGenTextures( 1, &texId3 );
    glBindTexture( GL_TEXTURE_CUBE_MAP, texId3 );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image4.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image5.data());
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image6.data());
    glGenerateMipmap( GL_TEXTURE_CUBE_MAP );
    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR
        );
    glTexParameteri(
        GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

}

static cy::Vec3f computeProjectorLookAt(cy::Vec4f pos_rot) {
    return pos_rot.XYZ();
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
    prog.BuildFiles("shader.vert", "shader.frag", NULL, "tcsShader.gs", "tesShader.gs");
    if(prog.IsNull()){
        fprintf(stderr, "Error creating shader program. \n");
        return 1;
    }
    cy::GLSLProgram prog1;
    prog1.CreateProgram();
    prog1.BuildFiles("shader1.vert", "shader1.frag", "shader.gs", "tcsShader1.gs", "tesShader1.gs");
    if(prog1.IsNull()){
        fprintf(stderr, "Error creating shader program. \n");
        return 1;
    }
    cy::GLSLProgram progEnv;
    progEnv.CreateProgram();
    progEnv.BuildFiles("shaderEnv.vert", "shaderEnv.frag");
    
    cy::GLSLProgram progObj;
    progObj.CreateProgram();
    progObj.BuildFiles("shaderObj.vert", "shaderObj.frag");
    
    cy::TriMesh mesh;
    cy::TriMesh meshLight;
    
    if(argc > 1){
        std::string ptr(args[1]);
        rootDir = ptr.substr(0, ptr.find_last_of('/')+1);
        bool success = mesh.LoadFromFileObj(args[1]);
        bool success1 = meshLight.LoadFromFileObj((rootDir + "light2.obj").c_str());
        mesh.ComputeNormals();
        meshLight.ComputeNormals();
    }
    /* initializations */
    createVertexArray();
    createVertexBufferForPlane();
    createVertexBufferForLight(&meshLight);
    createVertexBufferForTriangle();
    createVertexBufferForObject(&mesh);
    std::string ptr(args[2]);
    setNormalTextureData(ptr);
    std::string ptr1(args[3]);
    setDisplacementTextureData(ptr1);
    setEnvTextureData();
    setEnvTextureData1();
//    if(!createFrameBuffer()) return -1;
    //Plane
    GLint mvp_pos = glGetUniformLocation( prog.GetID(), "mvp" );
    GLint mv_pos = glGetUniformLocation(prog.GetID(), "mv");
    GLint normalMatrix_pos = glGetUniformLocation(prog.GetID(), "normalMatrix");
    GLint vpMatrix_pos = glGetUniformLocation(prog.GetID(), "vpMatrix");
    GLint projectorMatrix_pos = glGetUniformLocation(prog.GetID(), "projectorMatrix");
    GLuint normal_location = glGetAttribLocation(prog.GetID(), "normal");
    GLint lightDir = glGetUniformLocation(prog.GetID(), "lightDir");
    GLint cameraPos = glGetUniformLocation(prog.GetID(), "cameraPos");
    GLint shininess = glGetUniformLocation(prog.GetID(), "shininess");
    GLint visibility_pos = glGetUniformLocation(prog.GetID(), "visibility");
    GLint wavetime_Pos = glGetUniformLocation(prog.GetID(), "wavetime");
    GLint amp1_Pos = glGetUniformLocation(prog.GetID(), "amp1");
    GLint amp2_Pos = glGetUniformLocation(prog.GetID(), "amp2");
    GLint amp3_Pos = glGetUniformLocation(prog.GetID(), "amp3");
    GLuint pos_location = glGetAttribLocation(prog.GetID(), "pos");
    GLint sampler_t = glGetUniformLocation(prog.GetID(), "tex");
    GLint sampler_d = glGetUniformLocation(prog.GetID(), "tex1");
    GLint sampler_e1 = glGetUniformLocation(prog.GetID(), "env");
    GLuint txc_location = glGetAttribLocation(prog.GetID(), "txc");
    GLint sampler3 = glGetUniformLocation(prog.GetID(), "rockReflection");
    //Wireframe
    GLint cameraPos1 = glGetUniformLocation(prog1.GetID(), "cameraPos");
    GLint wavetime1 = glGetUniformLocation(prog1.GetID(), "wavetime");
    GLint amp1_Pos1 = glGetUniformLocation(prog1.GetID(), "amp1");
    GLint amp2_Pos1 = glGetUniformLocation(prog1.GetID(), "amp2");
    GLint amp3_Pos1 = glGetUniformLocation(prog1.GetID(), "amp3");
    GLint sampler_d1 = glGetUniformLocation(prog1.GetID(), "tex1");
    GLuint txc_location1 = glGetAttribLocation(prog1.GetID(), "txc");
    GLint tes_level = glGetUniformLocation(prog.GetID(), "tLevel");
    GLint tes_level1 = glGetUniformLocation(prog1.GetID(), "tLevel");
    GLint mvp_pos1 = glGetUniformLocation( prog1.GetID(), "mvp" );
    GLuint pos_location1 = glGetAttribLocation(prog1.GetID(), "pos");
    GLint vpMatrix_pos1 = glGetUniformLocation(prog1.GetID(), "vpMatrix");
    GLint projectorMatrix_pos1 = glGetUniformLocation(prog1.GetID(), "projectorMatrix");
    //Environment map
    GLint inverse_pos = glGetUniformLocation(progEnv.GetID(), "inverse");
    GLuint pos_location2 = glGetAttribLocation(progEnv.GetID(), "pos");
    GLint center = glGetUniformLocation(progEnv.GetID(), "center");
    GLint sampler_e = glGetUniformLocation(progEnv.GetID(), "env");
    //Rock
    GLint mvp_pos2 = glGetUniformLocation(progObj.GetID(), "mvp");
    GLint mv_pos1 = glGetUniformLocation(progObj.GetID(), "mv");
    GLint normalMatrix_pos1 = glGetUniformLocation(progObj.GetID(), "normalMatrix");
    GLint normalMatrix1_pos = glGetUniformLocation(progObj.GetID(), "normalMatrix1");
    GLint modelMatrix_pos = glGetUniformLocation(progObj.GetID(), "modelMatrix");
    GLuint pos_location3 = glGetAttribLocation(progObj.GetID(), "pos");
    GLuint normal_location2 = glGetAttribLocation(progObj.GetID(), "normal");
    GLint lightDir1 = glGetUniformLocation(progObj.GetID(), "lightDir");
    GLint cameraPos2 = glGetUniformLocation(progObj.GetID(), "cameraPos");
    GLint shininess1 = glGetUniformLocation(progObj.GetID(), "shininess");
    GLint mvp_pos3 = glGetUniformLocation(progObj.GetID(), "mvp");
    GLint mvp_pos4 = glGetUniformLocation(progObj.GetID(), "mvp");
    GLuint pos_location4 = glGetAttribLocation(progObj.GetID(), "pos");
    GLuint normal_location3 = glGetAttribLocation(progObj.GetID(), "normal");
    GLint diffuseColor = glGetUniformLocation(progObj.GetID(), "difCol");
    GLint specularColor = glGetUniformLocation(progObj.GetID(), "specCol");
    GLint diffuseColor1 = glGetUniformLocation(progObj.GetID(), "difCol");
    GLint specularColor1 = glGetUniformLocation(progObj.GetID(), "specCol");
    GLint ambientColor = glGetUniformLocation(progObj.GetID(), "ambCol");
    GLint ambientColor1 = glGetUniformLocation(progObj.GetID(), "ambCol");
    cy::Vec4f pos_rot;
    cy::Vec4f pos_projector;
    cy::Vec3f rockTranslation(-3.8,1.5,0.1);
//    cy::Vec3f rockTranslation(-3,1.5,-0.4);
//    cy::Vec3f lightTranslation(-3,1.5,-7.4);
    cy::Vec3f lightTranslation(-3,1.5,-12.4);
    cy::Vec3f diffuse(0.67,0.67,0.5);
    cy::Vec3f specular(0.0,0.0,0.0);
    cy::Vec3f ambient(0.2,0.2,0.2);
    cy::Vec3f diffuse1(0.0,0.0,0.0);
    cy::Vec3f specular1(0.0,0.0,0.0);
    cy::Vec3f ambient1(0.88,0.88,0.0);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
//        glDepthFunc(GL_LEQUAL);
        updateMousePositions(window);
        cy::Matrix4f rotMatrix = cy::Matrix4f::RotationXYZ(-yRot * (M_PI/180.0), -xRot * (M_PI/180.0), 0);
        cy::Matrix4f scaleMatrix = cy::Matrix4f::Scale(0.1);
        pos_rot = rotMatrix * cy::Vec4f(pos,1);
        pos_projector = cy::Vec4f(pos.x, pos.y + projectorHeightControl, pos.z, 1);
        cy::Matrix4f viewMatrix = cy::Matrix4f::View(pos_rot.XYZ() , trans.XYZ(), up.XYZ());
        cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(40.0 * (M_PI/180.0), 1920.0/1080.0, 0.1f, 1000.0f);
        cy::Vec3f pLookAt = computeProjectorLookAt(pos_rot);
        cy::Matrix4f viewMatrix1 = cy::Matrix4f::View(pos_projector.XYZ() , trans.XYZ(), up.XYZ());
        cy::Matrix4f viewMatrix2 = cy::Matrix4f::View(pos_projector.XYZ() , trans.XYZ(), up.XYZ()) * cy::Matrix4f::Scale(1,-1,1,1);
        cy::Matrix4f rot = cy::Matrix4f::RotationZ(-90 * M_PI/180.0);
        cy::Matrix4f modelMatrix = scaleMatrix;
        cy::Matrix4f transformationMatrix = projMatrix * viewMatrix1 * cy::Matrix4f::Scale(2.3) * cy::Matrix4f::Translation(rockTranslation) ;
        cy::Matrix4f transformationMatrix1 = projMatrix * viewMatrix * modelMatrix;
        cy::Matrix4f transformationMatrix2 = projMatrix * viewMatrix2 * cy::Matrix4f::Scale(2.3, 1.73, 2.3) * cy::Matrix4f::Translation(rockTranslation) ;
        
        cy::Matrix4f mvMatrix = viewMatrix * scaleMatrix;
        cy::Matrix3f normalMatrix = mvMatrix.GetSubMatrix3().GetInverse().GetTranspose();
        cy::Matrix3f normalMatrix1 =  modelMatrix.GetSubMatrix3().GetInverse().GetTranspose();
//        cy::Vec3f lightTrans(-xLightRot,19.0,-yLightRot);
        cy::Vec3f lightTrans(-xLightRot,-yLightRot, 13.0);
        cy::Vec3f lightTrans1(lightDirV.x - xLightRot,23.0,lightDirV.y - yLightRot);
        cy::Matrix4f translationLightMatrix = cy::Matrix4f::Translation(lightTrans.XYZ());
        cy::Vec4f rotLightDir = viewMatrix * translationLightMatrix * lightDirV;
        cy::Vec4f rotLightDir1 =  translationLightMatrix * lightDirV;
        cy::Matrix4f lightMatrix = cy::Matrix4f::View(rotLightDir1.XYZ() , trans.XYZ(), up.XYZ());
        cy::Matrix4f translationMatrix = projMatrix * viewMatrix * cy::Matrix4f::Scale(0.9) * cy::Matrix4f::Translation(rotLightDir1.XYZ());
        cy::Matrix4f transformationMatrix3 = projMatrix * viewMatrix1 * cy::Matrix4f::Scale(2.3) * cy::Matrix4f::Translation(lightTranslation);
        cy::Matrix4f vpMatrix = projMatrix * viewMatrix1;
        cy::Matrix4f rangeMatrix;
        rangeMatrix.SetRow(0, 1.1,   0,   0,  0.0);
        rangeMatrix.SetRow(1,   0, 0.9,   0, -0.3);
        rangeMatrix.SetRow(2,   0,   0, 1.0,  0.0);
        rangeMatrix.SetRow(3,   0,   0,   0,  1.0);
        cy::Matrix4f vpInverseMatrix = vpMatrix.GetInverse();
        cy::Matrix4f projectorMatrix = vpInverseMatrix * rangeMatrix;
        cy::Matrix4f inverseMatrix = projMatrix * viewMatrix;
        inverseMatrix = inverseMatrix.GetInverse();
        
        float matrix[16];
        float matrix1[16];
        float matrix2[9];
        float matrix3[16];
        float matrix5[16];
        float matrix4[9];
        
        /* environment mapping*/
        glUseProgram(progEnv.GetID());
        inverseMatrix.Get(matrix1);
        glUseProgram(progEnv.GetID());
        glUniformMatrix4fv(inverse_pos, 1, GL_FALSE, matrix1);
        glUseProgram(progEnv.GetID());
        glBindBuffer(GL_ARRAY_BUFFER, tri_vertexbuffer);
        glEnableVertexAttribArray(pos_location2);
        glVertexAttribPointer(pos_location2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glUseProgram(progEnv.GetID());
        glUniform3f(center, pos.x,pos.y,pos.z);
        glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, texId3 );
        glUseProgram(progEnv.GetID());
        glUniform1i(sampler_e, 0);

        glClearColor(1,1,1,1);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glUseProgram(progEnv.GetID());
        glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, texId3 );
        glDepthMask( GL_FALSE );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDepthMask( GL_TRUE );
        
        /* Rock rendering*/
        glUseProgram(progObj.GetID());
        transformationMatrix.Get(matrix);
        glUseProgram(progObj.GetID());
        glUniformMatrix4fv(mvp_pos2, 1, GL_FALSE, matrix);
        mvMatrix.Get(matrix1);
        glUseProgram(progObj.GetID());
        glUniformMatrix4fv(mv_pos1, 1, GL_FALSE, matrix1);
        normalMatrix.Get(matrix2);
        glUseProgram(progObj.GetID());
        glUniformMatrix3fv(normalMatrix_pos1, 1, GL_FALSE, matrix2);
        normalMatrix1.Get(matrix3);
        glUseProgram(progObj.GetID());
        glUniformMatrix3fv(normalMatrix1_pos, 1, GL_FALSE, matrix3);
        modelMatrix.Get(matrix4);
        glUseProgram(progObj.GetID());
        glUniformMatrix3fv(modelMatrix_pos, 1, GL_FALSE, matrix4);
        glUseProgram(progObj.GetID());
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBindBuffer(GL_ARRAY_BUFFER, qvbo);
        glEnableVertexAttribArray(pos_location3);
        glVertexAttribPointer(pos_location3, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glUseProgram(progObj.GetID());
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
//        glBindBuffer(GL_ARRAY_BUFFER, qvbo1);
        glEnableVertexAttribArray(normal_location2);
        glVertexAttribPointer(normal_location2, 3, GL_FLOAT, GL_FALSE,
                                  0, (void*) 0);
        glUseProgram(progObj.GetID());
        glUniform3f(lightDir1, lightTrans.x,lightTrans.y,lightTrans.z);
        glUniform3f(diffuseColor, diffuse.x,diffuse.y,diffuse.z);
        glUniform3f(specularColor, specular.x,specular.y,specular.z);
        glUniform3f(ambientColor, ambient.x,ambient.y,ambient.z);
        glUseProgram(progObj.GetID());
        //std::cout << "Camera position : " << pos_rot.x << pos_rot.y <<pos_rot.z;
        glUniform3f(cameraPos2, pos_rot.x,pos_rot.y,pos_rot.z);
        glUseProgram(progObj.GetID());
        glUniform1f(shininess1, 10.0);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(progObj.GetID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
        if(altFlag)
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        
        glUseProgram(progObj.GetID());
        transformationMatrix2.Get(matrix5);
        glUniformMatrix4fv(mvp_pos3, 1, GL_FALSE, matrix5);
        glUseProgram(progObj.GetID());
        cy::GLRenderTexture2D renderBuffer;
        renderBuffer.Initialize( true, 3, width, height);
        renderBuffer.Bind();
        renderBuffer.SetTextureWrappingMode(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
        if(altFlag)
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        renderBuffer.Unbind();
        renderBuffer.BuildTextureMipmaps();
        
        /* light object*/
        glUseProgram(progObj.GetID());
        transformationMatrix3.Get(matrix);
        glUniformMatrix4fv(mvp_pos4, 1, GL_FALSE, matrix);
        glUniform3f(diffuseColor1, diffuse1.x,diffuse1.y,diffuse1.z);
        glUniform3f(specularColor1, specular1.x,specular1.y,specular1.z);
        glUniform3f(ambientColor1, ambient1.x,ambient1.y,ambient1.z);
        glBindBuffer(GL_ARRAY_BUFFER, lvbo);
        glEnableVertexAttribArray(pos_location4);
        glVertexAttribPointer(pos_location4, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, lvbo1);
        glEnableVertexAttribArray(normal_location3);
        glVertexAttribPointer(normal_location3, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer_light);
        glEnable(GL_DEPTH_TEST);
        if(altFlag)
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        
        /* plane rendering*/
        glUseProgram(prog.GetID());
        transformationMatrix1.Get(matrix);
        glUniformMatrix4fv(mvp_pos, 1, GL_FALSE, matrix);
        mvMatrix.Get(matrix1);
        glUniformMatrix4fv(mv_pos, 1, GL_FALSE, matrix1);
        normalMatrix1.Get(matrix2);
        glUniformMatrix3fv(normalMatrix_pos, 1, GL_FALSE, matrix2);
        projectorMatrix.Get(matrix3);
        glUniformMatrix4fv(projectorMatrix_pos, 1, GL_FALSE, matrix3);
        vpMatrix.Get(matrix5);
        glUniformMatrix4fv(vpMatrix_pos, 1, GL_FALSE, matrix5);
        glUniform3f(lightDir, lightTrans.x,lightTrans.y,lightTrans.z);
        glUniform3f(cameraPos, pos_rot.x,pos_rot.y,pos_rot.z);
        glUniform1f(shininess, 100.0);
        wavetime += 0.1;
        glUniform1f(wavetime_Pos, wavetime);
        glUniform1f(amp1_Pos, amp1);
        glUniform1f(amp2_Pos, amp2);
        glUniform1f(amp3_Pos, amp3);
        glUniform1i(tes_level, tLevel);
        glUniform1i(visibility_pos, altFlag);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(prog.GetID());
        glBindBuffer(GL_ARRAY_BUFFER, qvbo);
        glEnableVertexAttribArray(pos_location);
        glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, qvbo1);
        glEnableVertexAttribArray(normal_location);
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, txc);
        glEnableVertexAttribArray(txc_location);
        glVertexAttribPointer(txc_location, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);
        glUniform1i(sampler_t, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texId1);
        glUniform1i(sampler_d, 1);
        glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
        glActiveTexture( GL_TEXTURE2 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, texId2);
        glUseProgram(prog.GetID());
        glUniform1i(sampler_e1, 2);
        
        glUseProgram(prog.GetID());
        glActiveTexture(GL_TEXTURE3);
        //glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glUniform1i(sampler3, 3);
        //glEnable(GL_DEPTH_TEST);
        renderBuffer.BindTexture(3);
        glPatchParameteri( GL_PATCH_VERTICES, 4 );
        glDrawArrays(GL_PATCHES, 0, 4);
        
        /* Wireframe*/
        glUseProgram(prog1.GetID());
        transformationMatrix1.Get(matrix);
        glUniformMatrix4fv(mvp_pos1, 1, GL_FALSE, matrix);
        projectorMatrix.Get(matrix3);
        glUniformMatrix4fv(projectorMatrix_pos1, 1, GL_FALSE, matrix3);
        vpMatrix.Get(matrix5);
        glUniformMatrix4fv(vpMatrix_pos1, 1, GL_FALSE, matrix5);
        glBindBuffer(GL_ARRAY_BUFFER, qvbo);
        glEnableVertexAttribArray(pos_location1);
        glVertexAttribPointer(pos_location1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, txc);
        glEnableVertexAttribArray(txc_location1);
        glVertexAttribPointer(txc_location1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glUniform1i(tes_level1, tLevel);
        glUniform1f(amp1_Pos1, amp1);
        glUniform1f(amp2_Pos1, amp2);
        glUniform1f(amp3_Pos1, amp3);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texId1);
        glUniform1i(sampler_d1, 1);
        if(spaceFlag){
        glDepthFunc(GL_ALWAYS);
        glPatchParameteri( GL_PATCH_VERTICES, 4 );
        glDrawArrays(GL_PATCHES, 0, 4);
        glDepthFunc(GL_LEQUAL);
        }
    
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
