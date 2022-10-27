#ifndef LINE_H
#define LINE_H


#include "Core/VisualObject.h"

class Line : public VisualObject
{
public:
    Line(const glm::vec3& direction, const glm::mat4& positionMatrix, glm::vec3 color, Shader& shader);
    void init() override;
    void draw() override;
    void scale(float s);

    //Must be set!
    const glm::vec3* mDirection{nullptr};
    const glm::mat4* mObjPosition{nullptr};
};

#endif // LINE_H
