

#ifndef SILHOUETTE_TECHNIQUE_H
#define	SILHOUETTE_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"

class SilhouetteTechnique : public Technique {
public:

    SilhouetteTechnique();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);
    void SetWorldMatrix(const Matrix4f& WVP);
    void SetLightPos(const Vector3f& Pos);

private:
    
    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_lightPosLocation;
};


#endif	/* SILHOUETTE_TECHNIQUE_H */
