#ifndef H_Cone
#define H_Cone

#include <glm/glm.hpp>
#include "SceneObject.h"

class Cone : public SceneObject
{

private:
    glm::vec3 origin;
    float radlength;
    float length;

public:
    Cone()
        : origin(glm::vec3(0)), radlength(1), length(1)
    {
        color = glm::vec3(1);
    };

    Cone(glm::vec3 o, float rl, float h, glm::vec3 col)
        : origin(o), radlength(rl), length(h)
    {
        color = col;
    };

    float intersect(glm::vec3 pos, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif //!H_Cone
