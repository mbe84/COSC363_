#include "Cone.h"
#include <math.h>

float Cone::intersect(glm::vec3 pos, glm::vec3 dir)
{
    glm::vec3 d = pos - origin;
    float minT;
    float maxT;
    float in1 = (dir.x*dir.x)+(dir.z*dir.z)-((radlength/length)*(radlength/length)*(dir.y*dir.y));
    float in2 = 2*(d.x*dir.x + d.z*dir.z+(radlength/length)*(radlength/length)*(length-pos.y+origin.y)*dir.y);
    float in3 = (d.x*d.x)+(d.z*d.z)-((radlength/length)*(radlength/length)*((length-pos.y+origin.y)*(length-pos.y+origin.y)));
    float t1 = (-in2 - sqrt(in2*in2 - 4*(in1*in3)))/(2*in1);
    float t2 = (-in2 + sqrt(in2*in2 - 4*(in1*in3)))/(2*in1);

    if(fabs(in2*in2 - 4*(in1*in3)) < 0.0010){
        return -1.;
    }
    if(in2*in2 - 4*(in1*in3) < 0.00){
         return -1.;
    }

    if((-in2 - sqrt(in2*in2 - 4*(in1*in3)))/(2*in1)<0.010){
        t1=-1;

    }
    if((-in2 + sqrt(in2*in2 - 4*(in1*in3)))/(2*in1)<0.010){
        t2=-1;
        minT = t2;
        maxT=t1;
    }

    if (t1<t2){
        minT = t1;
        maxT=t2;
    }

    float yaxis = pos.y + dir.y*minT;
    if((yaxis >= origin.y) && (yaxis <= origin.y + length)){
        return minT;
    }
    else{
        float yaxis = pos.y + dir.y*maxT;
        if((yaxis >= origin.y) && (yaxis <= origin.y + length)){
            return maxT;
        }else{
            return -1.;
    }
}

}
/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cone.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
    glm::vec3 d = p-origin;
    float r = sqrt(d.x * d.x + d.z * d.z);
    glm::vec3 n= glm::vec3 (d.x, r*(radlength/length), d.z);
    n=glm::normalize(n);
    return n;
}
