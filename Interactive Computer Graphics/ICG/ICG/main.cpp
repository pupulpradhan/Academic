//
//  main.cpp
//  ICG
//
//  Created by Pupul Pradhan on 1/28/22.
//

#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "cyTriMesh.h"
#include "cyGL.h"

float r = 0.0f;
float g = 0.0f;
float b = 0.0f;
GLuint vao;
GLuint vbo;

static void createVertexArray(){
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}
static void createVertexBuffer(cy::TriMesh *mesh){
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*mesh->NV(), &mesh->V(0), GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void render(){
    //GLuint pos = glGetAttribLocation(program, "pos");
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_POINTS, 0, 3241);
    glDisableVertexAttribArray(0);
    glutSwapBuffers();
}

void keyboardPressed(unsigned char key, int x, int y){
    if(key == 27)
    {
        glutDestroyWindow(glutGetWindow());
        _exit(0);
    }
}

void animateBackground(){
    r += 0.001f;
    g += 0.003f;
    b += 0.007f;
    if (r > 1.0)
    r = 0;
    if (g > 1.0)
    g = 0;
    if (b > 1.0)
    b = 0;

    glClearColor(r,g,b,1);
    glutPostRedisplay();
}

int main( int argc, char* args[] )
{
    //glutInitContextVersion(4,5);
    //glutInitContextFlags(GLUT_DEBUG);
    glutInit( &argc, args );
    glutInitContextVersion(4, 1);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize( 1920, 1080 );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "My ICG Window" );
    //CY_GL_REGISTER_DEBUG_CALLBACK;
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    
    glClearColor(0,0,0,1);              //Red
    createVertexArray();
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    cy::TriMesh mesh;
    if(argc > 1){
        bool success = mesh.LoadFromFileObj(args[1]);
    }
    createVertexBuffer(&mesh);
    std::cout << "GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "OPenGL version" << glGetString(GL_VERSION);
    cy::GLSLProgram prog;
    prog.CreateProgram();
    if(prog.IsNull()){
        fprintf(stderr, "Error creating shader program. \n");
        return 1;
    }
    prog.BuildFiles("shader.vert", "shader.frag");
    prog.Bind();
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboardPressed);
    //glutIdleFunc(animateBackground);
    glutMainLoop();

    return 0;
}
