#ifndef H_CYLINDER
#define H_CYLINDER

#include <glm/glm.hpp>
#include "SceneObject.h"

/**
 * Defines a simple Cylinder located at 'center'
 * with the specified radius
 */
class Cylinder : public SceneObject
{

private:
    glm::vec3 origin;
    float radLength;
    float length;

public:
    Cylinder()
        : origin(glm::vec3(0)), radLength(1), length(1)
    {
        color = glm::vec3(1);
    };

    Cylinder(glm::vec3 o, float rl, float l, glm::vec3 col)
        : origin(o), radLength(rl), length(l)
    {
        color = col;
    };

    float intersect(glm::vec3 pos, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif //!H_CYLINDER
