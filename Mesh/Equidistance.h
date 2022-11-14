#pragma once
#ifndef EQUIDISTANCE_H
#define EQUIDISTANCE_H

#include "core/VisualObject.h"

class Equidistance : public VisualObject
{
public:
    Equidistance(std::string materialName);
    Equidistance(std::string materialName, QVector3D a, QVector3D b);
    void insertLine(QVector3D a, QVector3D b);
    void init() override;
    void draw() override;

private:
    QMatrix4x4 mMatrix1;
};

#endif // EQUIDISTANCE_H
