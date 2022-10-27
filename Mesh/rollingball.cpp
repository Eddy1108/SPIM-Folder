#include "rollingball.h"
#include <QtMath>
#include "LAZSurface.h"

RollingBall::RollingBall(int n, QVector3D startPos, Shader& shader, VisualObject* surface) : OctahedronBall(shader, n)
{
    mSurface = surface;

    //Important! shader program name must be given
    mMatrix1.setToIdentity();

    mPos.setToIdentity();
    mPos.translate(startPos);

    mScale1.setToIdentity();
    mScale1.scale(mRadius, mRadius, mRadius);

    mMatrix1 = mPos * mScale1;
    //Debug line
//    mVelocityLine = new Line(mVelocity, mPosition, QVector3D(0,0,0));
//    mAccelerationLine = new Line(mAcceleration, mPosition, QVector3D(0,1,0));
    //mVelocityLine->scale(0.1);
    //mAccelerationLine->scale(1);
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    //std::cout << "deltaTime:" << dt << std::endl;
    int trianglesBallIsWithin{ 0 };
    std::vector<QVector3D> verticesPos = dynamic_cast<LAZSurface*>(mSurface)->getTriangleVertices(getPosition());

    if (3 == verticesPos.size())
    {
        QVector3D* v0 = &verticesPos.at(0);
        QVector3D* v1 = &verticesPos.at(1);
        QVector3D* v2 = &verticesPos.at(2);

        //beregn normal  // Kunne vært lagret i minne, slikt at vi slipper å kalkulere det hver render.
        QVector3D triangleNormal = findNormal(*v0, *v1, *v2);

        QVector3D currentPos = getPosition();

        float distance1 = QVector3D::dotProduct(triangleNormal, currentPos - *v1) - mRadius;
        //std::cout << "distance1: " << distance1 << std::endl;
        //std::cout << "Velocity: " << mVelocity << std::endl;
        //std::cout << "Acceleration: " << mAcceleration << std::endl;

        // Check if ball is within z axis, or moved below triangle.
        if (distance1 <= 0.0f)
        {
            trianglesBallIsWithin++;

            //beregn akselerasjonvektor - ligning (7)
            float cosAlpha = QVector3D::dotProduct(QVector3D(0, 0, 1), triangleNormal); // Finner vinkel mellom normal til xy-aksen og bakken sin normal

            QVector3D N_force = triangleNormal * abs(mGravity).z() * mMass * cosAlpha;

            //std::cout << "N Force: " << N_force.x() << ", " << N_force.y() << N_force.z() << std::endl;
            //std::cout << "Cos A: " << cos(cosAlpha) << std::endl;

            //Oppdatere hastighet og posisjon, ink. deltaTime
            QVector3D newAcc = (N_force + (mGravity * mMass)) / mMass;
            //std::cout << "Acceleration: " << mAcceleration.x() << ", " << mAcceleration.y() << ", " << mAcceleration.z() << std::endl;

            QVector3D newVel = mVelocity + (newAcc + mAcceleration) * (dt * 0.5f); // Multiplying here to reduce speed when testing
            //QVector3D newFriction = -newVel.normalized() * mGroundFriction;
            //newVel += newFriction;
            //std::cout << "dt: " << std::to_string(dt) << std::endl;
            mPos.translate((mVelocity.x() * dt + mAcceleration.x() * (pow(dt, 2) * 0.5f)) * timeSlowDown,
                (mVelocity.y() * dt + mAcceleration.y() * (pow(dt, 2) * 0.5f)) * timeSlowDown,
                (mVelocity.z() * dt + mAcceleration.z() * (pow(dt, 2) * 0.5f)) * timeSlowDown);

            //Update variables in memory.
            mVelocity = newVel;
            mAcceleration = newAcc;
            //Fix clipping
            QVector3D p_0 = *v1;
            QVector3D p = getPosition();

            float distance = QVector3D::dotProduct(triangleNormal, (p - p_0));
            //std::cout << "distance: " << distance << std::endl;

            //Moves the ball up, so that it snaps to the triangle
            if (sqrt(distance * distance) <= mRadius/* && sqrt(distance * distance) >= 0*/) {
                QVector3D offsetResult = triangleNormal * (mRadius - distance);
                mPos.translate(offsetResult.x(), offsetResult.y(), offsetResult.z());
            }

            //std::cout << "Velocity: " << mVelocity << std::endl;


            //int newTriangleIndex = i;

            // Flip Velocity
            // Ball has crossed over to a new triangle
            if (oldTriangleIndex == -1)
            {
                //Ball Bounces when it hits platform from freefall
                QVector3D VelocityAfter = mVelocity - triangleNormal * QVector3D::dotProduct(mVelocity, triangleNormal) * 2;

                //std::cout << "FLIP!" << std::endl;
                //std::cout << "Velocity: " << mVelocity.x() << ", " << mVelocity.y() << ", " << mVelocity.z() << std::endl;
                //std::cout << "VelocityAfter: " << VelocityAfter.x() << ", " << VelocityAfter.y() << ", " << VelocityAfter.z() << std::endl;
                mVelocity = VelocityAfter;
            }
            else
                if (1/*newTriangleIndex*/ != oldTriangleIndex) //ny indeks != forrige index
                {
                    //QVector3D distanceTraveled = mVelocity * *dt;

                    // beregn normalen til kollisjonsplanet, se ligning (9)
                    QVector3D collisionPlaneNormal = (triangleNormal + oldNormal) / (triangleNormal + oldNormal).length();
                    //std::cout << "CollisionPlanenormal " << collisionPlaneNormal.x() << ", " << collisionPlaneNormal.y() << ", " << collisionPlaneNormal.z() << std::endl;
                    // Korrigere posisjon oppover i normalens retning


                    QVector3D VelocityAfter = mVelocity - collisionPlaneNormal * (QVector3D::dotProduct(mVelocity, collisionPlaneNormal)) * 2;

                    //VelocityAfter = VelocityAfter + QVector3D(0,0,-3.3);

                    //std::cout << "Velocity: " << mVelocity.x() << ", " << mVelocity.y() << ", " << mVelocity.z() << std::endl;
                    //std::cout << "VelocityAfter: " << VelocityAfter.x() << ", " << VelocityAfter.y() << ", " << VelocityAfter.z() << std::endl;
                    mVelocity = VelocityAfter;

                    //QVector3D m = QVector3D::cross(vertices[oldTriangleIndex].to3DVec()*vertices[oldTriangleIndex+1].to3DVec(),vertices[oldTriangleIndex].to3DVec()*vertices[oldTriangleIndex+2].to3DVec());
                    //QVector3D normalvector = {(m+thisNormalVector)/(m+thisNormalVector).length()};
                    // Oppdater hastighetsvektoren, se ligning (8)
                    // Oppdater posisjon i retning den nye hastighetsvektoren
                }
            //oldTriangleIndex = newTriangleIndex;
            oldNormal = triangleNormal;
        }
    }
    // Freefall
    if (trianglesBallIsWithin == 0) {
        QVector3D newAcc = mGravity;

        QVector3D newVel = mVelocity + (newAcc + mAcceleration) * (dt * 0.5f); // Multiplying here to reduce speed when testing
        mPos.translate((mVelocity.x() * dt + mAcceleration.x() * (pow(dt, 2) * 0.5f)) * timeSlowDown,
            (mVelocity.y() * dt + mAcceleration.y() * (pow(dt, 2) * 0.5f)) * timeSlowDown,
            (mVelocity.z() * dt + mAcceleration.z() * (pow(dt, 2) * 0.5f)) * timeSlowDown);

        //Update variables.
        mAcceleration = newAcc;
        mVelocity = newVel;

        //std::cout << "Acceleration: " << mAcceleration.x() << ", " << mAcceleration.y() << ", " << mAcceleration.z() << std::endl;
        //std::cout << "Velocity: " << mVelocity.x() << ", " << mVelocity.y() << ", " << mVelocity.z() << std::endl;

        oldTriangleIndex = -1;
    }


    mMatrix1 = mPos * mScale1;
    return;
}

QVector3D RollingBall::findNormal(QVector3D v0, QVector3D v1, QVector3D v2)
{
    QVector3D ac = v2 - v0;
    QVector3D ab = v1 - v0;
    QVector3D result = QVector3D::crossProduct(ab, ac);

    return result.normalized();
}

QVector3D RollingBall::abs(QVector3D vector)
{
    QVector3D w;
    w.setX(sqrt(pow(vector.x(), 2)));
    w.setY(sqrt(pow(vector.y(), 2)));
    w.setZ(sqrt(pow(vector.z(), 2)));
    return w;
}
void RollingBall::setSurface(LAZSurface* surface)
{
    mSurface = surface;
}

QVector3D RollingBall::getPosition()
{
    return QVector3D(mPos(0, 3), mPos(1, 3), mPos(2, 3));
}

void RollingBall::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //Debug Line
 //   mVelocityLine->init(matrixUniform);
 //   mAccelerationLine->init(matrixUniform);
}

void RollingBall::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mShader.mMatrixUniform, 1, GL_FALSE, mMatrix1.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
    glBindVertexArray(0);
    
    //mVelocityLine->draw();
    //mAccelerationLine->draw();
}
