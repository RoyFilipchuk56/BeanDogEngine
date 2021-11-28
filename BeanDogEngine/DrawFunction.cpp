#include "GLCommon.h"       // Gives us glad.h

#include "cMesh.h"
#include "cVAOManager.h"
#include "GlobalItems.h"

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <iostream>

void SetUpTextures(cMesh* pCurrentMesh, GLuint shaderProgram)
{
    // HACK: To catch the debug 
    //if (pCurrentMesh->meshName == "Mercurio_xyz_n_rgba_x100_Bigger_perturbed_surface_xyz_n_rgba_uv.ply")
    //{
    //    shaderProgram = -1;
    //}

    const GLuint FIRST_TEXTURE_UNIT = 0;
    GLint isTransparent = glGetUniformLocation(shaderProgram, "isTransparent");
    glUniform1f(isTransparent, (GLfloat)GL_FALSE);
    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[0] >= 0.0f)
    {
        GLuint TextureNumber = gTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[0]);

        GLuint textureUnit = FIRST_TEXTURE_UNIT;	// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // TODO: Move elsewear for each one loaded
        GLint texture_00_LocID = glGetUniformLocation(shaderProgram, "texture_00");
        glUniform1i(texture_00_LocID, textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[1] >= 0.0f)
    {
        GLuint TextureNumber = gTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[1]);

        GLuint textureUnit = FIRST_TEXTURE_UNIT + 1;    // Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint texture_01_LocID = glGetUniformLocation(shaderProgram, "texture_01");
        glUniform1i(texture_01_LocID, textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[2] >= 0.0f)
    {
        GLuint TextureNumber = gTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[2]);

        GLuint textureUnit = FIRST_TEXTURE_UNIT + 2;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint texture_02_LocID = glGetUniformLocation(shaderProgram, "texture_02");
        glUniform1i(texture_02_LocID, textureUnit);
    }
    //*****************************************************************************************    

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[3] >= 0.0f)
    {
        GLuint TextureNumber = gTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[3]);

        GLuint textureUnit = FIRST_TEXTURE_UNIT + 3;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint texture_03_LocID = glGetUniformLocation(shaderProgram, "texture_03");
        glUniform1i(texture_03_LocID, textureUnit);
    }
    //*****************************************************************************************    

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[4] >= 0.0f)
    {
        GLuint TextureNumber = gTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[4]);

        GLuint textureUnit = FIRST_TEXTURE_UNIT + 4;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint texture_04_LocID = glGetUniformLocation(shaderProgram, "texture_04");
        glUniform1i(texture_04_LocID, textureUnit);
    }
    //*****************************************************************************************
    
    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[5] >= 0.0f)
    {
        GLuint TextureNumber = gTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[5]);

        GLuint textureUnit = FIRST_TEXTURE_UNIT + 5;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint texture_05_LocID = glGetUniformLocation(shaderProgram, "texture_05");
        glUniform1i(texture_05_LocID, textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[6] >= 0.0f)
    {
        GLuint TextureNumber = gTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[6]);

        GLuint textureUnit = FIRST_TEXTURE_UNIT + 6;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint texture_06_LocID = glGetUniformLocation(shaderProgram, "texture_06");
        glUniform1i(texture_06_LocID, textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[7] > 0.0f)
    {
        //Set Transparency to true
        glUniform1f(isTransparent, (GLfloat)GL_TRUE);

        GLuint TextureNumber = gTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[7]);

        GLuint textureUnit = FIRST_TEXTURE_UNIT + 7;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint texture_07_LocID = glGetUniformLocation(shaderProgram, "texture_07");
        glUniform1i(texture_07_LocID, textureUnit);
    }
    //*****************************************************************************************

    // Set all the texture ratios in the shader
    GLint textureRatios0to3_LocID = glGetUniformLocation(shaderProgram, "texture2D_Ratios0to3");
    // Set them
    glUniform4f(textureRatios0to3_LocID,
        pCurrentMesh->textureRatios[0],
        pCurrentMesh->textureRatios[1],
        pCurrentMesh->textureRatios[2],
        pCurrentMesh->textureRatios[3]);

    // Set all the texture ratios in the shader
    GLint textureRatios4to7_LocID = glGetUniformLocation(shaderProgram, "texture2D_Ratios4to7");
    // Set them
    glUniform4f(textureRatios4to7_LocID,
        pCurrentMesh->textureRatios[4],
        pCurrentMesh->textureRatios[5],
        pCurrentMesh->textureRatios[6],
        pCurrentMesh->textureRatios[7]);

    //Get the texture
    GLuint TextureNumber = ::gTextureManager->getTextureIDFromName("Jungle");

    //My cube map starts at unit 40
    GLuint textureUnit = 40;
    glActiveTexture(textureUnit + GL_TEXTURE0);

    //Bind it to a cube map not a 2D Texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureNumber);

    //TODO: Once again, please move this
    GLint cubeMap_00_LocID = glGetUniformLocation(shaderProgram, "cubeMap_00");
    glUniform1i(cubeMap_00_LocID, textureUnit);

    return;
}

//TODO: Add this to a graphics engine/manager
void DrawObject(cMesh* curMesh, glm::mat4 matModel, GLint matModel_Location, GLint matModelInverseTranspose_Location, GLuint program, cVAOManager* pVAOManager)
{
    //Set up the texures for the current mesh
    SetUpTextures(curMesh, program);

    // Get the uniform (should be outside of the draw call)
    GLint wholeObjectAlphaTransparency_LocID = glGetUniformLocation(program, "wholeObjectAlphaTransparency");
    // Set this value here
    glUniform1f(wholeObjectAlphaTransparency_LocID, curMesh->alphaTransparency);

    // Translate or "move" the object somewhere
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
        curMesh->transformXYZ);

    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
        curMesh->rotationXYZ.z,//(float)glfwGetTime(),
        glm::vec3(0.0f, 0.0f, 1.0f));

    curMesh->matRotationZ = rotateZ;

    // Rotation around the Y axis
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
        curMesh->rotationXYZ.y,
        glm::vec3(0.0f, 1.0f, 0.0f));

    curMesh->matRotationY = rotateY;

    // Rotation around the X axis
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
        curMesh->rotationXYZ.x,
        glm::vec3(1.0f, 0.0f, 0.0f));

    curMesh->matRotationX = rotateX;

    // Scale the model
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
        glm::vec3(curMesh->scale,  // Scale in X
            curMesh->scale,  // Scale in Y
            curMesh->scale));// Scale in Z


    matModel = matModel * matTranslate;
    matModel = matModel * rotateZ;
    matModel = matModel * rotateY;
    matModel = matModel * rotateX;
    matModel = matModel * matScale;

    glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));

    // Used to calculate the normal location in vertex space, using only rotation
    glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
    glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));

    // If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour"
    GLint bUseDebugColour_Location = glGetUniformLocation(program, "bUseDebugColour");
    GLint objectDebugColour_Location = glGetUniformLocation(program, "objectDebugColour");

    // If true, then the lighting contribution is NOT used
    GLint bDontLightObject_Location = glGetUniformLocation(program, "bDontLightObject");

    // The "whole object" colour (diffuse and specular)
    GLint wholeObjectDiffuseColour_Location = glGetUniformLocation(program, "wholeObjectDiffuseColour");
    GLint bUseWholeObjectDiffuseColour_Location = glGetUniformLocation(program, "bUseWholeObjectDiffuseColour");
    GLint wholeObjectSpecularColour_Location = glGetUniformLocation(program, "wholeObjectSpecularColour");


    if (curMesh->bUseWholeObjectDiffuseColour)
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_TRUE);
        glUniform4f(wholeObjectDiffuseColour_Location,
            curMesh->wholeObjectDiffuseRGBA.r,
            curMesh->wholeObjectDiffuseRGBA.g,
            curMesh->wholeObjectDiffuseRGBA.b,
            curMesh->wholeObjectDiffuseRGBA.a);
    }
    else
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_FALSE);
    }

    glUniform4f(wholeObjectSpecularColour_Location,
        curMesh->wholeObjectSpecularRGB.r,
        curMesh->wholeObjectSpecularRGB.g,
        curMesh->wholeObjectSpecularRGB.b,
        curMesh->wholeObjectShininess_SpecPower);

    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (curMesh->bUseObjectDebugColour)
    {
        // Override the colour...
        glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
        glUniform4f(objectDebugColour_Location,
            curMesh->objectDebugColourRGBA.r,
            curMesh->objectDebugColourRGBA.g,
            curMesh->objectDebugColourRGBA.b,
            curMesh->objectDebugColourRGBA.a);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
    }

    // See if mesh wants to be lit
    if (curMesh->bDontLight)
    {
        // Override the light
        glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the light
        glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
    }

    //Check if the 1 button has been pressed
    if (curMesh->bIsWireframe)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        //Draw everything with only lines
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        //Fill up those polys
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    sModelDrawInfo modelInfo;

    if (gVAOManager->FindDrawInfoByModelName(curMesh->meshName, modelInfo))
    {
        glBindVertexArray(modelInfo.VAO_ID);

        glDrawElements(GL_TRIANGLES,
            modelInfo.numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0);

        glBindVertexArray(0);
    }
}
