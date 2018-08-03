#include "Cylinder.h"
#include <math.h>

float Cylinder::intersect(glm::vec3 pos, glm::vec3 dir)
{
    glm::vec3 d = pos - origin;
    float a = (dir.x*dir.x)+(dir.z*dir.z);
    float b = 2*(dir.x*d.x+dir.z*d.z);
    float c = d.x*d.x+d.z*d.z-(radLength*radLength);

    float delta = b*b - 4*(a*c);

    if(delta < 0.0){
        return -1.0;
    }

    if((fabs(delta)) < 0.0010)
    {
        return -1.0;
    }

    float ypos = pos.y + dir.y*(-b + sqrt(delta))/(2 * a);
    if((ypos >= origin.y) && (ypos <= origin.y + length)){
        return (-b + sqrt(delta))/(2 * a);
    }
    else{
        float ypos = pos.y + dir.y*(-b - sqrt(delta))/(2 * a);
        if((ypos >= origin.y) && (ypos <= origin.y + length)){
            return (-b - sqrt(delta))/(2 * a);
        }else{
            return -1.0;
        }
    }

}


glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 d = p - origin;
    glm::vec3 n = glm::vec3 (d.x,0,d.z);
    n = glm::normalize(n); //normalize
    return n;

}
