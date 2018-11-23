//
// Created by monty on 27/01/16.
//

#ifndef LESSON09_STAR_H
#define LESSON09_STAR_H


class Star {
public:
    glm::vec4 mColor;
    glm::mat4 mTransform;

    Star( const glm::vec3& aPosition, const glm::vec4& aColour );
};


#endif //LESSON09_STAR_H
