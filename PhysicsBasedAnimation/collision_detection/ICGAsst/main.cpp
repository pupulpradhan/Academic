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
bool ctrlFlag = false;
bool altFlag = false;
bool initialFlag = true;
int width = 2920;
int height = 1980;
unsigned int imgWidth, imgHeight;
unsigned int textureWidth = 640;
unsigned int textureHeight = 480;
double xRot = 0.0f;
double yRot = 0.0f;
double xTrans = 0.0f;
double yTrans = 0.0f;
double initial_press_locX = 0.0f;
double initial_press_locY = 0.0f;
double final_release_locX = 0.0f;
double final_release_locY = 0.0f;
double rot_angle;
double rot_angle_new;
double rot_angle_prev;
float force_mag;
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
cy::Vec3f trans(1.75,1.75,0.0);
cy::Vec3f up(0.0,1.0,0.0);
cy::Vec3f trans_t(0.0,0.0,0.0);
cy::Vec3f up_t(0.0,1.0,0.0);
cy::Vec3f pos(0.0,0.0,-20.0);
cy::Vec3f pos_t(0.0,0.0,-15.0);
cy::Vec3f min(0.0,0.0,0.0);
cy::Vec3f max(1.0,1.0,1.0);
cy::Vec3f min1(0.0,0.0,0.0);
cy::Vec3f max1(1.0,1.0,1.0);
cy::Vec3f min2(0.0,0.0,0.0);
cy::Vec3f max2(1.0,1.0,1.0);
cy::Vec3f centre(0.5,0.5,0.5 - 0.0000066);
cy::Vec3f planePos(11.5,11.5,11.5);
cy::Vec3f lightDirV(33.0, 19.0, -35.0);
cy::Vec3f sphere_pos(1.0,1.0,0.0);
cy::Vec3f sphere_pos2(-65.0,1.0,0.0);
cy::Vec3f sphere_vel_intial(0.0,0.0,0.0);
cy::Vec3f sphere_vel_final(0.0,0.0,0.0);
cy::Vec3f sphere_velocity(0.0,0.0,0.0);
cy::Vec3f acceleration(0.0,0.0,0.0);
cy::Vec3f explicitVelocity(0.0,0.0,0.0);
cy::Vec3f implicitVelocity(0.0,0.0,0.0);
std::string rootDir;
double shadWidth = 5920;
double shadHeight = 3080;
float sphereRadius = 0.0f;

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

static void createVertexBufferForSphere(cy::TriMesh *mesh){
    // Vertex buffer for Teapot
    
    cy::TriMesh::TriFace faceArray[mesh->NF()];
    cy::TriMesh::TriFace faceTextureArray[mesh->NF()];
    cy::Vec3f vertPosArray[mesh->NV() * 10];
    cy::Vec3f vertNormArray[mesh->NVN() * 10];
    cy::Vec3f vertTexArray[mesh->NV() * 10];
    cy::Vec3f sum(0.0f);
    cy::Vec3f centroid(0.0f);
    float max_distance_squared = 0.0f;
    
    for (int i=0; i < mesh->NF(); i++ ){
        faceArray[i] = mesh->F(i);
    }
//    for (int i=0; i < mesh->NF(); i++ ){
//        faceTextureArray[i] = mesh->FT(i);
//    }
    for (int i=0; i < mesh->NV(); i++ ){
        vertPosArray[i] = mesh->V(i);
        sum += mesh->V(i);
    }
    // Divide by the number of vertices to get the centroid
    centroid =  sum / mesh->NV();
    for (int i=0; i < mesh->NV(); i++ ){
        float distance_squared = (mesh->V(i) - centroid).Length();
        max_distance_squared = std::max(max_distance_squared, distance_squared);
    }
    sphereRadius = max_distance_squared;
    for (int i=0; i < mesh->NVN(); i++ ){
        vertNormArray[i] = mesh->VN(i);
    }
    for (int i=0; i < mesh->NVT(); i++ ){
        vertTexArray[i].Set(0.0,0.0,0.0);
    }
    
    int k = mesh->NV();
//    for (int i=0; i < mesh->NF(); i++ ){
//        for (int j=0; j < 3; j++ ){
//            int l = faceTextureArray[i].v[j];
//            int m = faceArray[i].v[j];
//
//            if(m != l){
//                vertTexArray[k] = mesh->VT(l);
//                vertPosArray[k] = vertPosArray[m];
//                vertNormArray[k] = vertNormArray[m];
//                faceArray[i].v[j] = k;
//                k++;
//            }
//            else{
//                vertTexArray[m] = mesh->VT(l);
//            }
//        }
//    }
    
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
static void createVertexBufferForLight(cy::TriMesh *meshLight){
    // Vertex buffer for Light
    
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
        if(altFlag){
            glfwGetCursorPos(window, &newXRot, &newYRot);
            xRot = xRot + newXRot - prevX;
            yRot = yRot + newYRot - prevY;
            prevX = newXRot;
            prevY = newYRot;
        }
        else{
            glfwGetCursorPos(window, &newXRot, &newYRot);
            xTrans = xTrans + newXRot - prevX;
            yTrans = yTrans + newYRot - prevY;
            if(initialFlag){
                initial_press_locX = newXRot;
                initial_press_locY = newYRot;
                sphere_vel_intial = cy::Vec3f(initial_press_locX, initial_press_locY, 0.0 );
                initialFlag = false;
            }
            prevX = newXRot;
            prevY = newYRot;
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        glfwGetCursorPos(window, &newX, &newY);
        prevX = newX;
        prevY = newY;
        prevXRot = newX;
        prevYRot = newY;
        if(!initialFlag){
            final_release_locX = newX;
            final_release_locY = newY;
            sphere_vel_final = cy::Vec3f(final_release_locX, final_release_locY, 0.0 );
            sphere_velocity = sphere_vel_final - sphere_vel_intial;
            
            initialFlag = true;
        }
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

//bool CheckAABBIntersection(cy::Vec3f min1, cy::Vec3f max1, cy::Vec3f min2, cy::Vec3f max2){
//// If the minimum x-coordinate of box a is greater than the maximum x-coordinate of box b, or vice versa, they do not intersect
//    if (min1.x > max2.x || min2.x > max1.x) {
//        return false;
//    }
//
//    // Same for y and z coordinates
//    if (min1.y > max2.y || min2.y > max1.y) {
//        return false;
//    }
//
//    if (min1.z > max2.z || min2.z > max1.z) {
//        return false;
//    }
//
//    // If none of the above conditions are true, the AABBs intersect
//    return true;
//}
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
    window = glfwCreateWindow(width, height, "My PBA Window", NULL, NULL);
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
        mesh.LoadFromFileObj(args[1]);
        mesh.ComputeBoundingBox();
        meshLight.LoadFromFileObj((rootDir + "light.obj").c_str());
        mesh.ComputeNormals();
        meshLight.ComputeNormals();
    }
    /* initializations */
    createVertexArray();
    createVertexBufferForSphere(&mesh);
    createVertexBufferForLight(&meshLight);
    
    GLint matrixShadow_pos = glGetUniformLocation( prog.GetID(), "matrixShadow" );
    GLint mvp_pos1 = glGetUniformLocation(prog.GetID(), "mvp");
    GLint mv_pos = glGetUniformLocation(prog.GetID(), "mv");
    GLint force_mag_pos = glGetUniformLocation(prog.GetID(), "force_mag");
    GLint normalMatrix_pos = glGetUniformLocation(prog.GetID(), "normalMatrix");
    GLint modelMatrix_pos = glGetUniformLocation(prog.GetID(), "modelMatrix");
    GLuint pos_location = glGetAttribLocation(prog.GetID(), "pos");
    GLuint normal_location = glGetAttribLocation(prog.GetID(), "normal");
    GLint lightDir = glGetUniformLocation(prog.GetID(), "lightDir");
    GLint cameraPos = glGetUniformLocation(prog.GetID(), "cameraPos");
    GLint shininess = glGetUniformLocation(prog.GetID(), "shininess");
    GLuint normal_location1 = glGetAttribLocation(prog.GetID(), "normal");
    GLint mvp_pos2 = glGetUniformLocation(prog.GetID(), "mvp");
    GLuint pos_location3 = glGetAttribLocation(prog.GetID(), "pos");
    
    cy::Vec4f pos_rot;
    min = mesh.GetBoundMin();
    max = mesh.GetBoundMax();
    
//    std::cout<<"sphereRadius = "<< sphereRadius;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        updateMousePositions(window);
        //rot_angle = atan(final_release_locX - initial_press_locY / final_release_locX - initial_press_locX);
        rot_angle = atan2(sphere_velocity.y , sphere_velocity.x);
        force_mag = sqrt(pow(sphere_velocity.x, 2.0) + pow(sphere_velocity.y, 2.0));
        
        if(sphere_pos.x >= 500 || sphere_pos.x <= -340){
            sphere_velocity.x = -sphere_velocity.x;
        }
        if(sphere_pos.y >= 230 || sphere_pos.y <=-170){
            sphere_velocity.y = -sphere_velocity.y;
        }
        if(sphere_pos2.x >= 500 || sphere_pos2.x <= -340){
            sphere_velocity.x = -sphere_velocity.x;
        }
        if(sphere_pos2.y >= 230 || sphere_pos2.y <=-170){
            sphere_velocity.y = -sphere_velocity.y;
        }
        if((sphere_pos - sphere_pos2).Length() <= sphereRadius * 2 ){
            sphere_velocity.x = -sphere_velocity.x;
            sphere_velocity.y = -sphere_velocity.y;
        }
        
//        min1 += sphere_pos;
//        max1 += sphere_pos;
//        min2 += sphere_pos2;
//        max2 += sphere_pos2;
//        bool flag = CheckAABBIntersection(min1, max1, min2, max2);
//        if(flag){
//            sphere_velocity.x = -sphere_velocity.x;
//            sphere_velocity.y = -sphere_velocity.y;
//        }
        
//        std::cout<< "sphere_pos : " << sphere_pos.x << " " <<  sphere_pos.y ;
        sphere_pos.x = sphere_pos.x - sphere_velocity.x * 0.002; // x0 + vt
        sphere_pos.y = sphere_pos.y - sphere_velocity.y * 0.002;
        sphere_pos2.x = sphere_pos2.x - sphere_velocity.x * 0.02; // x0 + vt
        sphere_pos2.y = sphere_pos2.y - sphere_velocity.y * 0.02;
        //std::cout<<"force_mag "<<force_mag<<"\n";
        
        cy::Matrix4f rotMatrix = cy::Matrix4f::RotationXYZ(-yRot * (M_PI/180.0), -xRot * (M_PI/180.0), 0);
        cy::Matrix4f scaleMatrix_sphere = cy::Matrix4f::Scale(0.03);
        pos_rot = rotMatrix * cy::Vec4f(pos,1);
        cy::Matrix4f viewMatrix = cy::Matrix4f::View(pos_rot.XYZ() , trans.XYZ(), up.XYZ());
        cy::Matrix4f projMatrix = cy::Matrix4f::Perspective(40.0 * (M_PI/180.0), 1920.0/1080.0, 0.1f, 1000.0f);
        cy::Matrix4f projMatrix1 = cy::Matrix4f::Perspective(70.0 * (M_PI/180.0), 1920.0/1080.0, 0.1f, 1000.0f);
        cy::Matrix4f translationMatrix_sphere = cy::Matrix4f::Translation(sphere_pos);
        cy::Matrix4f translationMatrix_sphere2 = cy::Matrix4f::Translation(sphere_pos2);
        cy::Matrix4f modelMatrix = scaleMatrix_sphere * translationMatrix_sphere;
        cy::Matrix4f modelMatrix2 = scaleMatrix_sphere * translationMatrix_sphere2;
        cy::Matrix4f transformationMatrix1 = projMatrix * viewMatrix * modelMatrix ;
        cy::Matrix4f transformationMatrix2 = projMatrix * viewMatrix * modelMatrix2 ;
        cy::Matrix4f mvMatrix = viewMatrix * scaleMatrix_sphere;
        cy::Matrix3f normalMatrix = mvMatrix.GetSubMatrix3().GetInverse().GetTranspose();
        cy::Vec3f lightTrans(-xLightRot,19.0,-yLightRot);
        cy::Vec3f lightTrans1(lightDirV.x - xLightRot,23.0,lightDirV.y - yLightRot);
        cy::Matrix4f translationLightMatrix = cy::Matrix4f::Translation(lightTrans.XYZ());
        cy::Vec4f rotLightDir = viewMatrix * translationLightMatrix * lightDirV;
        cy::Vec4f rotLightDir1 =  translationLightMatrix * lightDirV;
        cy::Matrix4f lightMatrix = cy::Matrix4f::View(rotLightDir1.XYZ() , trans.XYZ(), up.XYZ());
        cy::Matrix4f translationMatrix = projMatrix1 * viewMatrix * cy::Matrix4f::Scale(0.1) * cy::Matrix4f::Translation(rotLightDir1.XYZ());
        cy::Matrix4f matrixMLP = projMatrix1 * lightMatrix * cy::Matrix4f::Scale(1.0);
        cy::Matrix4f matrixShadow = cy::Matrix4f::Translation(centre) * cy::Matrix4f::Scale(0.5) * matrixMLP;
        float matrix[16];
        float matrix1[16];
        float matrix2[9];
        float matrix4[9];

        /* Sphere rendering*/
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
        glUniform1f(shininess, 1500.0);
        glUniform3f(force_mag_pos, 0.0, 0.0, 0.0);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(prog.GetID());
        glActiveTexture(GL_TEXTURE0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        transformationMatrix2.Get(matrix);
        glUniformMatrix4fv(mvp_pos1, 1, GL_FALSE, matrix);
        glDrawElements(GL_TRIANGLES, sizeof(cy::Vec3f)*mesh.NF(), GL_UNSIGNED_INT, 0);
        
        /* light object*/
        glUseProgram(prog.GetID());
        translationMatrix.Get(matrix);
        glUniformMatrix4fv(mvp_pos2, 1, GL_FALSE, matrix);
        glUniform3f(force_mag_pos, 0.0, 0.0, 0.0);
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
