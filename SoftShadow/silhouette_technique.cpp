
#include <limits.h>
#include <string>


#include "silhouette_technique.h"
#include "ogldev_util.h"

using namespace std;

SilhouetteTechnique::SilhouetteTechnique()
{   
}


bool SilhouetteTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "C:\\Users\\ksampathkuma\\cubeshadowdemo\\SoftShadow\\shaders\\silhouette.vs")) {
        return false;
    }

    if (!AddShader(GL_GEOMETRY_SHADER, "C:\\Users\\ksampathkuma\\cubeshadowdemo\\SoftShadow\\shaders\\silhouette.gs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "C:\\Users\\ksampathkuma\\cubeshadowdemo\\SoftShadow\\shaders\\silhouette.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }
    
    m_WVPLocation = GetUniformLocation("gWVP");
    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    m_lightPosLocation = GetUniformLocation("gLightPos");

    return true;
}


void SilhouetteTechnique::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
}


void SilhouetteTechnique::SetWorldMatrix(const Matrix4f& WorldInverse)
{
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
}


void SilhouetteTechnique::SetLightPos(const Vector3f& Pos)
{
    glUniform3f(m_lightPosLocation, Pos.x, Pos.y, Pos.z);
}