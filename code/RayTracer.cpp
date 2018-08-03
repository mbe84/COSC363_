/*========================================================================
* COSC 363  Computer Graphics (2018)
* Ray tracer
* See Lab07.pdf for details.
*=========================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/glut.h>
#include "Plane.h"
#include "Cone.h"
#include "Cylinder.h"
#include "TextureBMP.h"
using namespace std;

TextureBMP texture;

const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;  //A global list containing pointers to objects in the scene


//---The most important function in a ray tracer! ----------------------------------
//   Computes the colour value obtained by tracing a ray and finding its
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
    glm::vec3 backgroundCol(0.15, 0.1, 0.1);
    glm::vec3 light(10, 40, -3);
    glm::vec3 light1(-30, 40, -3);
    glm::vec3 ambientCol(0.2);   //Ambient color of light

    ray.closestPt(sceneObjects);        //Compute the closest point of intersetion of objects with the ray

    if(ray.xindex == -1) return backgroundCol;      //If there is no intersection return background colour

    glm::vec3 materialCol = sceneObjects[ray.xindex]->getColor(); //else return object's colour


    //mine
    glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);

    glm::vec3 lightVector = light - ray.xpt;
    glm::vec3 lightVector1 = light1 - ray.xpt;

    lightVector = glm::normalize(lightVector);
    lightVector1 = glm::normalize(lightVector1);

    glm::vec3 reflVector = glm::reflect(-lightVector, normalVector);
    glm::vec3 reflVector1 = glm::reflect(-lightVector1, normalVector);

    float rDotV = glm::dot(reflVector, -ray.dir);
    float rDotV1 = glm::dot(reflVector1, -ray.dir);

    rDotV = max(rDotV, 0.0f);
    rDotV1 = max(rDotV1, 0.0f);

    glm::vec3 specVector = pow(rDotV, 10) * glm::vec3(1,1,1);
    glm::vec3 specVector1 = pow(rDotV1, 10) * glm::vec3(1,1,1);

    float lDotn = glm::dot(lightVector,normalVector);
    float lDotn1 = glm::dot(lightVector1,normalVector);

    Ray shadow(ray.xpt, lightVector);
    Ray shadow1(ray.xpt, lightVector1);
    shadow.closestPt(sceneObjects);
    shadow1.closestPt(sceneObjects);

    glm::vec3 colorSum(0);

    if(ray.xindex == 0 ){
        if((int)ray.xpt.z%7 == 1 || (int)ray.xpt.z%7 == -1){
            colorSum += glm::vec3(0.9, 0.8, 0);
        } else if((int)ray.xpt.x%7 == 1 || (int)ray.xpt.x%7 == -1){
            colorSum = glm::vec3(0.9, 0.8, 0);
        } else  {
            colorSum = glm::vec3(0.1, 0.2, 0);
        }

    }

    if(ray.xindex == 7 ){

        if(int(ray.xpt.x+ray.xpt.z-20)%2 == 0){
            colorSum = glm::vec3(0,0,0);
        } else  {
            if((int)ray.xpt.y%3 == 1|| (int)ray.xpt.y%3 == -1){
                colorSum = glm::vec3(0.8,0.5,0.);
            } else {
                colorSum = glm::vec3(0.25,0.25,0.25);
            }
        }

    }

    if(ray.xindex == 6 ){

        if((int)ray.xpt.x%3 == 1 || (int)ray.xpt.x%3 == -1){
            colorSum = glm::vec3(1,0,0);
        } else if((int)ray.xpt.x%3 == 0){
            colorSum = glm::vec3(0,1,0);
        } else {
            colorSum = glm::vec3(0,0,1);
        }


    }

    if(ray.xindex <= 4 && ray.xindex >= 1){

        if((int)ray.xpt.y%3 == 1 || (int)ray.xpt.y%3 == -1){
            colorSum = glm::vec3(0.5,0.5,0);
        } else if((int)ray.xpt.y%3 == 0){
            colorSum = glm::vec3(0,1,0);
        } else {
            if((int)ray.xpt.x%3 == 1|| (int)ray.xpt.x%3 == -1){
                colorSum = glm::vec3(0.5,0.2,0.);
            } else {
                colorSum = glm::vec3(0,0.5,0.5);
            }
        }



    }

    if(ray.xindex == 8)
    {
        float s = (ray.xpt.x+49)/99;
        float t = (ray.xpt.y+21)/71;
        colorSum = texture.getColorAt(s,t);
    }

    if(ray.xindex >=9){

        if((int)ray.xpt.y%3 == 1 || (int)ray.xpt.y%3 == -1){
            colorSum = glm::vec3(0.,0.9,07);
        } else if((int)ray.xpt.y%3 == 0){
            colorSum = glm::vec3(0.9,0.7,0);
        } else {
            colorSum = glm::vec3(0.7,0.0,0.9);
        }

        Ray transRay(ray.xpt, ray.dir);
        glm::vec3 transColor = trace(transRay, step+1);
        colorSum = 0.4f*colorSum + 0.6f*transColor;
    }



    if((lDotn < 0||lDotn1 < 0 || (shadow.xindex > -1 && (shadow.xdist < glm::length(light - ray.xpt)))|| (shadow1.xindex > -1 && (shadow1.xdist < glm::length(light1 - ray.xpt))))&& !(ray.xindex <= 4 && ray.xindex >= 1)){
        colorSum += ambientCol*materialCol;
    } else {
        colorSum += (ambientCol*materialCol) + (lDotn*materialCol) + specVector + specVector1;
    }
    if(ray.xindex >= 1 && step < MAX_STEPS && ray.xindex != 9 && !(ray.xindex <= 4 && ray.xindex >= 1) && ! (ray.xindex >=9)&& (ray.xindex !=8)){
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
        Ray reflectedRay(ray.xpt, reflectedDir);
        glm::vec3 reflectedCol = trace(reflectedRay, step+1); //Recursion!
        colorSum = colorSum + (0.8f*reflectedCol);
    }


    return colorSum;

}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
    float xp, yp;  //grid point
    float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
    float cellY = (YMAX-YMIN)/NUMDIV;  //cell height

    glm::vec3 eye(0., 0., 0.);  //The eye position (source of primary rays) is the origin

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);  //Each cell is a quad.

    for(int i = 0; i < NUMDIV; i++)     //For each grid point xp, yp
    {
        xp = XMIN + i*cellX;
        for(int j = 0; j < NUMDIV; j++)
        {
            yp = YMIN + j*cellY;

            glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);  //direction of the primary ray

            Ray ray = Ray(eye, dir);        //Create a ray originating from the camera in the direction 'dir'
            ray.normalize();                //Normalize the direction of the ray to a unit vector
            glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value

            glColor3f(col.r, col.g, col.b);
            glVertex2f(xp, yp);             //Draw each cell with its color value
            glVertex2f(xp+cellX, yp);
            glVertex2f(xp+cellX, yp+cellY);
            glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}

//---The cube drawing -----------------------------------------------------------
// This big boy will yeet a cube
//---------------------------------------------------------------------------------------
void Cube(float xaxis, float yaxis, float zaxis, float length, float width, float height, glm::vec3 colorVec)
{

    Plane *plane1 = new Plane(glm::vec3(xaxis,yaxis,zaxis),glm::vec3(xaxis+length,yaxis,zaxis),glm::vec3(xaxis+length,yaxis+height,zaxis),glm::vec3(xaxis,yaxis+height,zaxis),colorVec);
    sceneObjects.push_back(plane1);
    Plane *plane2 = new Plane(glm::vec3(xaxis+length,yaxis,zaxis),glm::vec3(xaxis+length,yaxis,zaxis-width),glm::vec3(xaxis+length,yaxis+height,zaxis-width),glm::vec3(xaxis+length,yaxis+height,zaxis),colorVec);
    sceneObjects.push_back(plane2);
    Plane *plane3 = new Plane(glm::vec3(xaxis+length,yaxis,zaxis-width),glm::vec3(xaxis,yaxis,zaxis-width),glm::vec3(xaxis,yaxis+height,zaxis-width),glm::vec3(xaxis+length,yaxis+height,zaxis-width),colorVec);
    sceneObjects.push_back(plane3);
    Plane *plane4 = new Plane(glm::vec3(xaxis,yaxis+height,zaxis),glm::vec3(xaxis,yaxis+height,zaxis-width),glm::vec3(xaxis,yaxis,zaxis-width),glm::vec3(xaxis,yaxis,zaxis),colorVec);
    sceneObjects.push_back(plane4);
    Plane *plane5 = new Plane(glm::vec3(xaxis,yaxis+height,zaxis),glm::vec3(xaxis+length,yaxis+height,zaxis),glm::vec3(xaxis+length,yaxis+height,zaxis-width),glm::vec3(xaxis,yaxis+height,zaxis-width),colorVec);
    sceneObjects.push_back(plane5);
    Plane *plane6 = new Plane(glm::vec3(xaxis,yaxis,zaxis-width),glm::vec3(xaxis+length,yaxis,zaxis-width),glm::vec3(xaxis+length,yaxis,zaxis),glm::vec3(xaxis,yaxis,zaxis),colorVec);
    sceneObjects.push_back(plane6);
}
//---The tetrahedron drawing -----------------------------------------------------------
// This big boy will yeet a tetrahedron
//---------------------------------------------------------------------------------------
void Tetrahedron(float xaxis, float yaxis, float zaxis, float length, glm::vec3 colorVec)
{
    Plane *triangle1 = new Plane(glm::vec3(xaxis+length/2,yaxis,zaxis),glm::vec3(xaxis+length,yaxis,zaxis),glm::vec3(xaxis+length*0.5, yaxis, zaxis + sqrt(3.0f) * 0.5* length ),glm::vec3(xaxis,yaxis,zaxis),colorVec); //AB,B,C,A
    sceneObjects.push_back(triangle1);
    Plane *triangle2 = new Plane(glm::vec3(xaxis+length,yaxis,zaxis),glm::vec3(xaxis+length*0.5, yaxis+sqrt(6.0f)/3.0f * length, zaxis + sqrt(3.0f) * 0.25* length ),glm::vec3((glm::vec3(xaxis+length*0.5, yaxis, zaxis + sqrt(3.0f) * 0.5* length ).x+glm::vec3(xaxis+length*0.5, yaxis+sqrt(6.0f)/3.0f * length, zaxis + sqrt(3.0f) * 0.25* length ).x)/2,(glm::vec3(xaxis+length*0.5, yaxis, zaxis + sqrt(3.0f) * 0.5* length ).y+glm::vec3(xaxis+length*0.5, yaxis+sqrt(6.0f)/3.0f * length, zaxis + sqrt(3.0f) * 0.25* length ).y)/2,(glm::vec3(xaxis+length*0.5, yaxis, zaxis + sqrt(3.0f) * 0.5* length ).z+glm::vec3(xaxis+length*0.5, yaxis+sqrt(6.0f)/3.0f * length, zaxis + sqrt(3.0f) * 0.25* length ).z)/2),glm::vec3(xaxis+length*0.5, yaxis, zaxis + sqrt(3.0f) * 0.5* length ),colorVec); //B,D,CD,C
    sceneObjects.push_back(triangle2);
    Plane *triangle3 = new Plane(glm::vec3(xaxis+length*0.5, yaxis+sqrt(6.0f)/3.0f * length, zaxis + sqrt(3.0f) * 0.25* length ),glm::vec3(xaxis,yaxis,zaxis),glm::vec3(xaxis+length*0.5, yaxis, zaxis + sqrt(3.0f) * 0.5* length ),glm::vec3((glm::vec3(xaxis+length*0.5, yaxis, zaxis + sqrt(3.0f) * 0.5* length ).x+glm::vec3(xaxis+length*0.5, yaxis+sqrt(6.0f)/3.0f * length, zaxis + sqrt(3.0f) * 0.25* length ).x)/2,(glm::vec3(xaxis+length*0.5, yaxis, zaxis + sqrt(3.0f) * 0.5* length ).y+glm::vec3(xaxis+length*0.5, yaxis+sqrt(6.0f)/3.0f * length, zaxis + sqrt(3.0f) * 0.25* length ).y)/2,(glm::vec3(xaxis+length*0.5, yaxis, zaxis + sqrt(3.0f) * 0.5* length ).z+glm::vec3(xaxis+length*0.5, yaxis+sqrt(6.0f)/3.0f * length, zaxis + sqrt(3.0f) * 0.25* length ).z)/2),colorVec); //D,A,C,CD
    sceneObjects.push_back(triangle3);
    Plane *triangle4 = new Plane(glm::vec3(xaxis,yaxis,zaxis),glm::vec3(xaxis+length*0.5, yaxis+sqrt(6.0f)/3.0f * length, zaxis + sqrt(3.0f) * 0.25* length ),glm::vec3(xaxis+length,yaxis,zaxis),glm::vec3(xaxis+length/2,yaxis,zaxis),colorVec); //A,D,B,AB
    sceneObjects.push_back(triangle4);
}


//---This function initializes the scene -------------------------------------------
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    texture = TextureBMP((char*)"void.bmp");
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);

    //-- Create a pointer to a sphere object
    Sphere *sphere1 = new Sphere(glm::vec3(3.0, 9., -110.0), 15.0, glm::vec3(0, 0, 1)); // 15.0, 9.0, -150.0
    //-- Create a pointer to a sphere object
    Sphere *sphere2 = new Sphere(glm::vec3(-12.0, 12.0, -90.0), 5.0, glm::vec3(1, 0, 0));
    //-- Create a pointer to a sphere object
    Sphere *sphere3 = new Sphere(glm::vec3(5.5, 3.5, -45.0), 2.0, glm::vec3(1, 0, 0));
    //-- Create a pointer to a cone object
    Cone *cone = new Cone(glm::vec3(20.0, -19.5, -100), 10, 15, glm::vec3(0.75, 0.25, 0.75));
    //-- Create a pointer to a cylinder object
    Cylinder *cylinder = new Cylinder(glm::vec3(-17.5, -19.5, -90.1), 7, 20, glm::vec3(0., 0., 0.));
    //-- Create a Pane
    Plane *plane = new Plane (glm::vec3(-40., -20, -40),//Point A
    glm::vec3(40., -20, -40),//Point B
    glm::vec3(40., -20, -500),//Point C
    glm::vec3(-40., -20, -500),//Point D
    glm::vec3(0.5, 0.5, 0.5));//Colour
    sceneObjects.push_back(plane);
    //HOLD POINT -22.5, -19.5, -90.1
    //-- Creating and pushing the Tetrahedron into the scene
    Tetrahedron(-6.0, -19.5, -90.1,15,glm::vec3(1, 0.7529, 0.7961));//-6.0, -19.5, -90.1



    sceneObjects.push_back(sphere1);
    sceneObjects.push_back(sphere2);
    //sceneObjects.push_back(sphere3);
    sceneObjects.push_back(cone);
    //sceneObjects.push_back(sphere1);
    //sceneObjects.push_back(sphere2);
    //sceneObjects.push_back(sphere3);
    sceneObjects.push_back(cylinder);
    //-- Creating and pushing the cube into the scene 10.5, -19.5, -90.0
    Cube(10.0, -19.0, -140.0,20,15,15,glm::vec3( 0.5, 1, 0.5)); //-3.0, -19.5, -110.0
    sceneObjects.push_back(sphere3);



}



int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
