
#ifndef OGLDEV_APP_H
#define OGLDEV_APP_H

#ifndef WIN32
#include "freetypeGL.h"
#endif


class OgldevApp
{
protected:
	OgldevApp();

	void CalcFPS();

	void RenderFPS();

	float GetRunningTime();

protected:
#ifndef WIN32
	FontRenderer m_fontRenderer;
#endif
private:
	long long m_frameTime;
	long long m_startTime;
	int m_frameCount;
        int m_fps;
};


#endif