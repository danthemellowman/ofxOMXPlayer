#include "ofMain.h"
#include "testApp.h"
#include "ofGLProgrammableRenderer.h"

int main()
{
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(720, 480, OF_WINDOW);
	ofRunApp( new testApp());
}