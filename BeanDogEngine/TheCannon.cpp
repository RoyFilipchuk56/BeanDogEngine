#include "TheCannon.h"
#include <iostream>
TheCannon::TheCannon(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation) : GameObject(mesh, transform, rotation)
{
    isMovingLeft = false;
    isMovingRight = false;
    isMovingUp = false;
    isMovingDown = false;

    myMesh = mesh;

    initialVec.x = 0;
    initialVec.y = 0;
    initialVec.z = 1;
    initialVec.w = 0;

    movementSpeed = 0.5f;
    pitchMin = -1.3744f;
    pitchMax = 0.65;
    yawMin = -1.5708f;
    yawMax = 1.5708f;
    projectileRadius = 0.5;
}
TheCannon::TheCannon(cMesh* mesh) : GameObject(mesh)
{
	isMovingLeft = false;
	isMovingRight = false;
	isMovingUp = false;
	isMovingDown = false;

	myMesh = mesh;
    
    initialVec.x = 0;
    initialVec.y = 0;
    initialVec.z = 1;
    initialVec.w = 0;

    movementSpeed = 0.5f;
    pitchMin = -1.3744f;
    pitchMax = 0.65;
    yawMin = -1.5708f;
    yawMax = 1.5708f;
    projectileRadius = 0.5;
}
TheCannon::TheCannon(glm::vec3 transform) : GameObject(transform)
{
    isMovingLeft = false;
    isMovingRight = false;
    isMovingUp = false;
    isMovingDown = false;

    myMesh = mesh;

    initialVec.x = 0;
    initialVec.y = 0;
    initialVec.z = 1;
    initialVec.w = 0;

    movementSpeed = 0.5f;
    pitchMin = -1.3744f;
    pitchMax = 0.65;
    yawMin = -1.5708f;
    yawMax = 1.5708f;
    projectileRadius = 0.5;
}
TheCannon::~TheCannon()
{

}

void TheCannon::Update(float deltaTime)
{
    if (isMovingLeft)
    {
        SetRotation(GetRotation() + glm::vec3(0, movementSpeed * deltaTime, 0));
    }
    if (isMovingRight)
    {
        SetRotation(GetRotation() - glm::vec3(0, movementSpeed * deltaTime, 0));
    }
    if (isMovingUp)
    {
        float tempRotation = glm::clamp<float>(GetRotation().x - (movementSpeed * deltaTime), pitchMin, pitchMax);
        glm::vec3 curRotation = GetRotation();
        curRotation.x = tempRotation;
        SetRotation(curRotation);
    }
    if (isMovingDown)
    {
        float tempRotation = glm::clamp<float>(GetRotation().x + (movementSpeed * deltaTime), pitchMin, pitchMax);
        glm::vec3 curRotation = GetRotation();
        curRotation.x = tempRotation;
        SetRotation(curRotation);
    }
}
void TheCannon::UpdateCommands(float deltaTime)
{
    if (commands != NULL)
    {
        commands->Update(deltaTime);
    }
}
void TheCannon::SetFriendlyName(std::string name)
{
    this->friendlyName = friendlyName;
}
std::string TheCannon::GetFriendlyName()
{
    return friendlyName;
}
cMesh* TheCannon::GetMesh()
{
    return mesh;
}
void TheCannon::SetMesh(cMesh* mesh)
{
    this->mesh = mesh;
    this->rotationXYZ = glm::vec3(glm::degrees(mesh->rotationXYZ.x), glm::degrees(mesh->rotationXYZ.y), glm::degrees(mesh->rotationXYZ.z));
    this->position = mesh->rotationXYZ;
}
nPhysics::cProjectile* TheCannon::ShootBullet()
{
    //Create a mesh for the bullet
    cMesh* bulletMesh = new cMesh;
    bulletMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
    bulletMesh->transformXYZ = GetPosition();
    bulletMesh->textureNames[0] = "IronBlock.bmp";
    bulletMesh->textureRatios[0] = 1.0f;
    
    //Create a projectile
    nPhysics::cProjectile* tempProjectile = new nPhysics::cProjectile(1, GetPosition(), bulletMesh);
    //Get the rotation matrix
    glm::mat4 tempRotation = myMesh->matRotationX * myMesh->matRotationY;
    //Multiply that by the initial position and make it a vec4
    glm::vec4 finalRotation = initialVec * myMesh->matRotationXYZ;
    //set the velocity and ignore the 4th value in the vec4
    tempProjectile->SetVelocity(glm::normalize(glm::vec3(-finalRotation.x, -finalRotation.y, finalRotation.z)) * 30.0f);
    //Set the acceleration
    //TODO: Change to a force regestry
    tempProjectile->SetAcceleration(glm::vec3(0, -9.8, 0));

    return tempProjectile;
}

void TheCannon::SetCannonConfig(float pitchMin, float pitchMax, float yawMin, float yawMax, float maxSpeed)
{
    this->movementSpeed = maxSpeed;
    this->pitchMin = pitchMin;
    this->pitchMax = pitchMax;
    this->yawMin = yawMin;
    this->yawMax = yawMax;
}

void TheCannon::SetProjectileSize(float radius)
{
}
