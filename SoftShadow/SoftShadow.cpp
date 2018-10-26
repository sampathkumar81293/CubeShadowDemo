
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <iostream>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif

#include "engine_common.h"
#include "ogldev_app.h"
#include "ogldev_camera.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "texture.h"
#include "silhouette_technique.h"
#include "ogldev_basic_lighting.h"
#include "ogldev_glut_backend.h"
#include "mesh.h"

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

class SoftShadow : public ICallbacks, public OgldevApp
{
public:

    SoftShadow() 
    {
        m_pGameCamera = NULL;
        m_scale = 0.0f;
        m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        m_directionalLight.AmbientIntensity = 0.55f;
        m_directionalLight.DiffuseIntensity = 0.9f;
        m_directionalLight.Direction = Vector3f(1.0f, 1.0f, 0.0);

        m_persProjInfo.FOV = 60.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 100.0f;  
        
       // m_boxPos = Vector3f(0.0f, 2.0f, 4.0f);
		//m_boxPos = Vector3f(0.0f, 2.0f, 0.0);
		m_meshOrientation.m_pos = Vector3f(0.0f, 2.0f, 4.0f);
		m_quadOrientation.m_scale = Vector3f(10.0f, 10.0f, 10.0f);
		m_quadOrientation.m_rotation = Vector3f(90.0f, 0.0f, 0.0f);
    }

    ~SoftShadow()
    {
        SAFE_DELETE(m_pGameCamera);
    }    

    bool Init()
    {
        Vector3f Pos(9.0f, 5.0f, -7.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);
        
        if (!m_silhouetteTech.Init()) {
            printf("Error initializing the silhouette technique\n");
            return false;            
        }
      
        if (!m_LightingTech.Init()) {
            printf("Error initializing the lighting technique\n");
            return false;
        }
        
        m_LightingTech.Enable();
        
        m_LightingTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
        m_LightingTech.SetDirectionalLight(m_directionalLight);
        m_LightingTech.SetMatSpecularIntensity(0.0f);
        m_LightingTech.SetMatSpecularPower(0);        

        if (!m_mesh.LoadMesh("../Content/box.obj", true)) {
            printf("Mesh load failed\n");
            return false;            
        }
        
#ifndef WIN32
        if (!m_fontRenderer.InitFontRenderer()) {
            return false;
        }
#endif        	
		if (!m_quad.LoadMesh("../Content/quad.obj", false))
		{
			return false;
		}
		m_pGroundTex = new Texture(GL_TEXTURE_2D, "../Content/test.png");

		if (!m_pGroundTex->Load()) { return false; }
		return true;
	}

    void Run()
    {
        GLUTBackendRun(this);
    }
         

    virtual void RenderSceneCB()
    {   
        CalcFPS();
        
        m_scale += 0.01f;
               
        m_pGameCamera->OnRender();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                      
        RenderScene();
		RenderBasePlane();
        RenderFPS();
        
        glutSwapBuffers();
    }


    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }
    

	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		default:
			m_pGameCamera->OnKeyboard(OgldevKey);
		}
	}

   
private:
           
    void RenderScene()
    {
        // Render the object as-is
        m_LightingTech.Enable();
                                     
        Pipeline p;
        p.SetPerspectiveProj(m_persProjInfo);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());        
		//p.WorldPos(m_pos);
		m_meshOrientation.m_rotation = Vector3f(0, m_scale, 0);
		p.Orient(m_meshOrientation);
        m_LightingTech.SetWorldMatrix(p.GetWorldTrans());        
        m_LightingTech.SetWVP(p.GetWVPTrans());        
        m_mesh.Render();
        
        // Render the object's silhouette
        m_silhouetteTech.Enable();
        
        m_silhouetteTech.SetWorldMatrix(p.GetWorldTrans());        
        m_silhouetteTech.SetWVP(p.GetWVPTrans());        
        m_silhouetteTech.SetLightPos(Vector3f(10.0f, 20.0f, 0.0f));
        
        glLineWidth(5.0f);
        
        m_mesh.Render();        
    } 


	void RenderBasePlane()
	{
		// Render the object as-is
		m_LightingTech.Enable();

		Pipeline p;
		p.SetPerspectiveProj(m_persProjInfo);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		// remder the plane surface here
		//		m_quadOrientation.m_rotation = Vector3f(0,m_scale,0);
		p.Orient(m_quadOrientation);
		m_LightingTech.SetWorldMatrix(p.GetWorldTrans());
		m_LightingTech.SetWVP(p.GetWVPTrans());
		m_pGroundTex->Bind(COLOR_TEXTURE_UNIT);
		m_quad.Render();
	}
    
    BasicLightingTechnique m_LightingTech;
    SilhouetteTechnique m_silhouetteTech;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_directionalLight;

	Mesh m_mesh;
	//Vector3f m_boxPos;
	Orientation m_meshOrientation;

	Mesh m_quad;
	Texture* m_pGroundTex;
	Orientation m_quadOrientation;

    PersProjInfo m_persProjInfo;
};


int main(int argc, char** argv)
{
//    Magick::InitializeMagick(*argv);
    GLUTBackendInit(argc, argv, true, false);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Computer Graphics Demo")) {
        return 1;
    }

    glDepthFunc(GL_LEQUAL);
    
	SRANDOM;
    
    SoftShadow* pApp = new SoftShadow();
	std::cout << "Debugging" << endl;
    if (!pApp->Init()) {
        return 1;
    }
    
    pApp->Run();

    delete pApp;
 
    return 0;
}
