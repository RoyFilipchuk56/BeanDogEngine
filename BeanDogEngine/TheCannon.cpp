#include "TheCannon.h"
#include <iostream>
TheCannon::TheCannon(cMesh* mesh)
{
	isMovingLeft = false;
	isMovingRight = false;
	isMovingUp = false;
	isMovingDown = false;

	myMesh = mesh;
	movementSpeed = 0.5f;
    
    initialVec.x = 0;
    initialVec.y = 0;
    initialVec.z = 1;
    initialVec.w = 0;
}
TheCannon::~TheCannon()
{

}

void TheCannon::Update(float deltaTime)
{
    if (isMovingLeft)
    {
        myMesh->rotationXYZ.y += movementSpeed * deltaTime;
    }
    if (isMovingRight)
    {
        myMesh->rotationXYZ.y -= movementSpeed * deltaTime;
    }
    if (isMovingUp)
    {
        float tempRotation = glm::clamp<float>(myMesh->rotationXYZ.x - (movementSpeed * deltaTime), -1.3744f, 0);
        myMesh->rotationXYZ.x = tempRotation;
    }
    if (isMovingDown)
    {
        float tempRotation = glm::clamp<float>(myMesh->rotationXYZ.x + (movementSpeed * deltaTime), -1.3744f, 0);
        myMesh->rotationXYZ.x = tempRotation;
    }
}
nPhysics::cProjectile* TheCannon::ShootBullet()
{
    //Create a mesh for the bullet
    cMesh* bulletMesh = new cMesh;
    bulletMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
    bulletMesh->transformXYZ = glm::vec3(0, 1, 0);
    
    //Create a projectile
    nPhysics::cProjectile* tempProjectile = new nPhysics::cProjectile(1, glm::vec3(0, 1, 0), bulletMesh);
    //Get the rotation matrix
    glm::mat4 tempRotation = myMesh->matRotationX * myMesh->matRotationY;
    //Multiply that by the initial position and make it a vec4
    glm::vec4 finalRotation = initialVec * tempRotation;
    //set the velocity and ignore the 4th value in the vec4
    tempProjectile->SetVelocity(glm::normalize(glm::vec3(-finalRotation.x, -finalRotation.y, finalRotation.z)) * 15.0f);
    //Set the acceleration
    //TODO: Change to a force regestry
    tempProjectile->SetAcceleration(glm::vec3(0, -9.8, 0));

    return tempProjectile;
}