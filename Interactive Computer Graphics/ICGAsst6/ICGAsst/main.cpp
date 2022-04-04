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
GLuint ebuffer1;
GLuint texId;
GLuint txc;
GLuint txc_t;
GLuint txc_p;
GLuint frameBuffer = 0;
GLuint depthBuffer;
GLuint renderedTexture;
GLuint tri_vertexbuffer;
GLuint quad_VertexArrayID;
GLuint quad_vertexbuffer;
GLint origFB;
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
cy::Vec3f up(0.0,10.0,0.0);
cy::Vec3f trans_t(0.0,0.0,0.0);
cy::Vec3f up_t(0.0,-1.0,0.0);
cy::Vec3f pos(0.0,0.0,-10.0);
cy::Vec3f pos_t(0.0,0.0,-15.0);
cy::Vec3f min(0.0,0.0,0.0);
cy::Vec3f max(1.0,1.0,1.0);
cy::Vec3f centre(0.0,0.0,0.5);
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
        std::cout << "recompile done \n";
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
                 0, GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, 0 );
    glGenerateMipmap( renderedTexture );
    glTexParameteri( GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glGenRenderbuffers( 1, &depthBuffer );
    glBindRenderbuffer( GL_RENDERBUFFER, depthBuffer );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height );
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer );
    glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0 );
    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers( 1, drawBuffers );
    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE ) return false;
    return true;
}

static void createVertexBufferForTriangle(cy::TriMesh *mesh){
    // Vertex buffer for CUBE
    
    static const GLfloat traingle_buffer_data[] = {
        -1.0f, -1.0f, 0.999f,
        3.0f, -1.0f, 0.999f,
        -1.0f,  3.0f, 0.999f
    };
    
    glGenBuffers(1, &tri_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tri_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(traingle_buffer_data), traingle_buffer_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*mesh->NV(), &mesh->V(0), GL_STATIC_DRAW);
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*mesh->NVN(), &mesh->VN(0), GL_STATIC_DRAW);
    glGenBuffers(1, &ebuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cy::Vec3f)*mesh->NF(), &mesh->F(0), GL_STATIC_DRAW);
    
}

static void createVertexBufferForPlane(cy::TriMesh *meshPlane){
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
        //if (altFlag){
            glfwGetCursorPos(window, &newX, &newY);
            xRot = xRot + newX - prevX;
            yRot = yRot + newY - prevY;
            prevX = newX;
            prevY = newY;
        //}
        if (ctrlFlag){
            glfwGetCursorPos(window, &newXRot, &newYRot);
            xLightRot = xLightRot + newXRot - prevXRot;
            yLightRot = yLightRot + newYRot - prevYRot;
            prevXRot = newXRot;
            prevYRot = newYRot;
        }
//        else{
//            glfwGetCursorPos(window, &newX_t, &newY_t);
//            xRot_t = xRot_t + newX_t - prevX_t;
//            yRot_t = yRot_t + newY_t - prevY_t;
//            prevX_t = newX_t;
//            prevY_t = newY_t;
//        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        glfwGetCursorPos(window, &newX, &newY);
        prevX = newX;
        prevY = newY;
        prevXRot = newX;
        prevYRot = newY;
        glfwGetCursorPos(window, &newX_t, &newY_t);
        prevX_t = newX_t;
        prevY_t = newY_t;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        //if (altFlag){
            glfwGetCursorPos(window, &newTransX, &newTransY);
            xTranslation = prevTransX - newTransX;
            prevTransX = newTransX;
            pos.z += 0.05 * xTranslation;
        //}
//        else{
//            glfwGetCursorPos(window, &newTransX_t, &newTransY_t);
//            xTranslation_t = prevTransX_t - newTransX_t;
//            prevTransX_t = newTransX_t;
//            pos_t.z += 0.05 * xTranslation_t;
//        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
        glfwGetCursorPos(window, &newTransX, &newTransY);
        prevTransX = newTransX;
        glfwGetCursorPos(window, &newTransX_t, &newTransY_t);
        prevTransX_t = newTransX_t;
    }
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

static void  setTextureData(){
    std::string filename1 = rootDir + "cubemap_posx.png";
    std::vector<unsigned char> image1 = loadImageToTex(filename1);
    std::string filename2 = rootDir + "cubemap_negx.png";
    std::vector<unsigned char> image2 = loadImageToTex(filename2);
    std::string filename3 = rootDir + "cubemap_posy.png";
    std::vector<unsigned char> image3 = loadImageToTex(filename3);
    std::string filename4 = rootDir + "cubemap_negy.png";
    std::vector<unsigned char> image4 = loadImageToTex(filename4);
    std::string filename5 = rootDir + "cubemap_posz.png";
    std::vector<unsigned char> image5 = loadImageToTex(filename5);
    std::string filename6 = rootDir + "cubemap_negz.png";
    std::vector<unsigned char> image6 = loadImageToTex(filename6);
    glGenTextures( 1, &texId );
    glBindTexture( GL_TEXTURE_CUBE_MAP, texId );
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
    prog.BuildFiles("shader1.vert", "shader1.frag");
    //prog.Bind();
    cy::GLSLProgram prog1;
    prog1.CreateProgram();
    prog1.BuildFiles("shader.vert", "shader.frag");
    //prog1.Bind();
    cy::GLSLProgram prog2;
    prog2.CreateProgram();
    prog2.BuildFiles("shader2.vert", "shader2.frag");
    //prog2.Bind();
    if(prog2.IsNull()){
        fprintf(stderr, "Error creating shader program. \n");
        return 1;
    }
    cy::TriMesh mesh;
    cy::TriMesh meshPlane;
    
    if(argc > 1){
        std::string ptr(args[1]);
        rootDir = ptr.substr(0, ptr.find_last_of('/')+1);
        //bool success = mesh.LoadFromFileObj((rootDir + "teapot_2.obj").c_str());
        bool success = mesh.LoadFromFileObj(args[1]);
        bool success1 = meshPlane.LoadFromFileObj((rootDir + "plane.obj").c_str());
        mesh.ComputeBoundingBox();
        mesh.ComputeNormals();
    }
    
    /* initializations */
    createVertexArray();
    createVertexBufferForPlane(&meshPlane);
    createVertexBufferForTriangle(&mesh);
    //if(!createFrameBuffer()) return -1;
    //createVertexBuffer(&mesh);
    setTextureData();
    
    cy::Vec4f pos_rot;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        updateMousePositions(window);
        cy::Matrix4f rotMatrix = cy::Matrix4f::RotationXYZ(-yRot * (M_PI/180.0), -xRot * (M_PI/180.0), 0);
        cy::Matrix4f rotMatrix1 = cy::Matrix4f::RotationXYZ(-180 * (M_PI/180.0), -180 * (M_PI/180.0), 0);
        //cy::Matrix4f translationMatrix = cy::Matrix4f::Translation(-centre.XYZ());
        cy::Matrix4f scaleMatrix = cy::Matrix4f::Scale(0.1);
        pos_rot = rotMatrix * cy::Vec4f(pos,1);
        cy::Matrix4f viewMatrix = cy::Matrix4f::View(pos_rot.XYZ() , trans.XYZ(), up.XYZ());
        cy::Matrix4f viewMatrix1 = cy::Matrix4f::View(pos_rot.XYZ() , trans.XYZ(), up.XYZ()) * cy::Matrix4f::Scale(1,-1,1,1);
        cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(40.0 * (M_PI/180.0), 1920.0/1080.0, 0.1f, 1000.0f);
        cy::Matrix4f modelMatrix = scaleMatrix;
        cy::Matrix4f modelMatrix1 = cy::Matrix4f::Scale(1);
        cy::Matrix4f transformationMatrix = projMatrix * viewMatrix * modelMatrix;
        cy::Matrix4f transformationMatrix1 = projMatrix * viewMatrix1 * modelMatrix;
        cy::Matrix3f normalMatrix1 =  modelMatrix.GetSubMatrix3().GetInverse().GetTranspose();
        cy::Matrix4f inverseMatrix = projMatrix * viewMatrix;
        inverseMatrix = inverseMatrix.GetInverse();
        cy::Matrix4f mvMatrix = viewMatrix * scaleMatrix;
        cy::Matrix4f mvMatrix1 = viewMatrix1 * scaleMatrix;
        cy::Matrix3f normalMatrix = mvMatrix.GetSubMatrix3().GetInverse().GetTranspose();
        float matrix[16];
        float matrix1[16];
        float matrix2[9];
        float matrix3[16];
        float matrix4[9];
        float matrix5[16];
        
        /* environment mapping*/
        inverseMatrix.Get(matrix1);
        GLint inverse_pos = glGetUniformLocation(prog.GetID(), "inverse");
        glUseProgram(prog.GetID());
        glUniformMatrix4fv(inverse_pos, 1, GL_FALSE, matrix1);
        GLuint pos_location = glGetAttribLocation(prog.GetID(), "pos");
        glUseProgram(prog.GetID());
        glBindBuffer(GL_ARRAY_BUFFER, tri_vertexbuffer);
        glEnableVertexAttribArray(pos_location);
        glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        GLint center = glGetUniformLocation(prog.GetID(), "center");
        glUseProgram(prog.GetID());
        glUniform3f(center, pos.x,pos.y,pos.z);
        glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, texId );
        GLint sampler = glGetUniformLocation(prog.GetID(), "env");
        glUseProgram(prog.GetID());
        glUniform1i(sampler, 0);

        glClearColor(1,1,1,1);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glUseProgram(prog.GetID());
        glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, texId );
        glDepthMask( GL_FALSE );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDepthMask( GL_TRUE );
        
        /* Teapot rendering*/
        glUseProgram(prog1.GetID());
        transformationMatrix.Get(matrix);
        GLint mvp_pos1 = glGetUniformLocation(prog1.GetID(), "mvp");
        glUseProgram(prog1.GetID());
        glUniformMatrix4fv(mvp_pos1, 1, GL_FALSE, matrix);
        mvMatrix.Get(matrix1);
        GLint mv_pos = glGetUniformLocation(prog1.GetID(), "mv");
        glUseProgram(prog1.GetID());
        glUniformMatrix4fv(mv_pos, 1, GL_FALSE, matrix1);
        normalMatrix.Get(matrix2);
        GLint normalMatrix_pos = glGetUniformLocation(prog1.GetID(), "normalMatrix");
        glUseProgram(prog1.GetID());
        glUniformMatrix3fv(normalMatrix_pos, 1, GL_FALSE, matrix2);
        normalMatrix1.Get(matrix3);
        GLint normalMatrix1_pos = glGetUniformLocation(prog1.GetID(), "normalMatrix1");
        glUseProgram(prog1.GetID());
        glUniformMatrix3fv(normalMatrix1_pos, 1, GL_FALSE, matrix3);
        modelMatrix.Get(matrix4);
        GLint modelMatrix_pos = glGetUniformLocation(prog1.GetID(), "modelMatrix");
        glUseProgram(prog1.GetID());
        glUniformMatrix3fv(modelMatrix_pos, 1, GL_FALSE, matrix4);
        GLuint pos_location1 = glGetAttribLocation(prog1.GetID(), "pos");
        glUseProgram(prog1.GetID());
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBindBuffer(GL_ARRAY_BUFFER, qvbo);
        glEnableVertexAttribArray(pos_location1);
        glVertexAttribPointer(pos_location1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        GLuint normal_location = glGetAttribLocation(prog1.GetID(), "normal");
        glUseProgram(prog1.GetID());
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
//        glBindBuffer(GL_ARRAY_BUFFER, qvbo1);
        glEnableVertexAttribArray(normal_location);
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE,
                                  0, (void*) 0);
        GLint lightDir = glGetUniformLocation(prog1.GetID(), "lightDir");
        glUseProgram(prog1.GetID());
        glUniform3f(lightDir, lightDirV.x,lightDirV.y,lightDirV.z);
        GLint cameraPos = glGetUniformLocation(prog1.GetID(), "cameraPos");
        glUseProgram(prog1.GetID());
        //std::cout << "Camera position : " << pos_rot.x << pos_rot.y <<pos_rot.z;
        glUniform3f(cameraPos, pos_rot.x,pos_rot.y,pos_rot.z);
        GLint shininess = glGetUniformLocation(prog1.GetID(), "shininess");
        glUseProgram(prog1.GetID());
        glUniform1f(shininess, 300.0);
        glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, texId );
        GLint sampler1 = glGetUniformLocation(prog1.GetID(), "env");
        glUseProgram(prog1.GetID());
        glUniform1i(sampler1, 0);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(prog1.GetID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        
        glUseProgram(prog1.GetID());
        transformationMatrix1.Get(matrix5);
        GLint mvp_pos2 = glGetUniformLocation(prog1.GetID(), "mvp");
        glUseProgram(prog1.GetID());
        glUniformMatrix4fv(mvp_pos2, 1, GL_FALSE, matrix5);
        
//        glViewport( 0, 0, width, height );
//        glClearColor(1.0,0.5,0.5,1);
//        glClear(  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        cy::GLRenderTexture2D renderBuffer;
        renderBuffer.Initialize( true, 3, 3030, 1830);
        renderBuffer.Bind();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        renderBuffer.Unbind();
        renderBuffer.BuildTextureMipmaps();

//        /* Plane Rendering */
        glUseProgram(prog2.GetID());
        transformationMatrix.Get(matrix);
        GLint mvp_pos3 = glGetUniformLocation(prog2.GetID(), "mvp");
        glUseProgram(prog2.GetID());
        glUniformMatrix4fv(mvp_pos3, 1, GL_FALSE, matrix);
        mvMatrix.Get(matrix1);
        GLint mv_pos1 = glGetUniformLocation(prog2.GetID(), "mv");
        glUseProgram(prog2.GetID());
        glUniformMatrix4fv(mv_pos1, 1, GL_FALSE, matrix1);
        normalMatrix.Get(matrix2);
        GLint normalMatrix_pos1 = glGetUniformLocation(prog2.GetID(), "normalMatrix");
        glUseProgram(prog2.GetID());
        glUniformMatrix3fv(normalMatrix_pos1, 1, GL_FALSE, matrix2);
        normalMatrix1.Get(matrix3);
        GLint normalMatrix1_pos1 = glGetUniformLocation(prog2.GetID(), "normalMatrix1");
        glUseProgram(prog2.GetID());
        glUniformMatrix3fv(normalMatrix1_pos1, 1, GL_FALSE, matrix3);
        modelMatrix1.Get(matrix4);
        GLint modelMatrix_pos1 = glGetUniformLocation(prog2.GetID(), "modelMatrix");
        glUseProgram(prog2.GetID());
        glUniformMatrix3fv(modelMatrix_pos1, 1, GL_FALSE, matrix4);
        GLuint pos_location2 = glGetAttribLocation(prog2.GetID(), "pos");
        glUseProgram(prog2.GetID());
        glBindBuffer(GL_ARRAY_BUFFER, qvbo);
        glEnableVertexAttribArray(pos_location2);
        glVertexAttribPointer(pos_location2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        GLuint normal_location1 = glGetAttribLocation(prog2.GetID(), "normal");
        glUseProgram(prog2.GetID());
        glBindBuffer(GL_ARRAY_BUFFER, qvbo1);
        glEnableVertexAttribArray(normal_location1);
        glVertexAttribPointer(normal_location1, 3, GL_FLOAT, GL_FALSE,
                                  0, (void*) 0);
        GLint lightDir1 = glGetUniformLocation(prog2.GetID(), "lightDir");
        glUseProgram(prog2.GetID());
        glUniform3f(lightDir1, lightDirV.x,lightDirV.y,lightDirV.z);
        GLint cameraPos1 = glGetUniformLocation(prog2.GetID(), "cameraPos");
        glUseProgram(prog2.GetID());
        glUniform3f(cameraPos1, pos_rot.x,pos_rot.y,pos_rot.z);
        GLint shininess1 = glGetUniformLocation(prog2.GetID(), "shininess");
        glUseProgram(prog2.GetID());
        glUniform1f(shininess1, 300.0);
        glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, texId);
        GLint sampler2 = glGetUniformLocation(prog2.GetID(), "env");
        glUseProgram(prog2.GetID());
        glUniform1i(sampler2, 0);
        
        GLint sampler3 = glGetUniformLocation(prog2.GetID(), "teapotReflection");
        glUseProgram(prog2.GetID());
        glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glUniform1i(sampler3, 1);
        //glEnable(GL_DEPTH_TEST);
        renderBuffer.BindTexture(1);
        glUseProgram(prog2.GetID());
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
