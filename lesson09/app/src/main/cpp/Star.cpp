//
// Created by monty on 27/01/16.
//
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Star.h"

Star::Star(const glm::vec3 &aPosition, const glm::vec4 &aColour):
    mTransform( glm::translate(glm::mat4(1.0f), aPosition ) ),
    mColor( aColour )
{
}
