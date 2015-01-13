#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"
#include "TerminalListener.h"

class testApp : public ofBaseApp, public KeyListener{

public:

	void setup();
	void update();
	void draw();
	void keyPressed(int key);	
	vector<ofxOMXPlayer*> omxPlayers;
	
	void onCharacterReceived(KeyListenerEventData& e);
	TerminalListener consoleListener;


    float  totalDuration;
    float  startTime;
    float  endTime;
    int  totalScreens;
    int  fade;
    int  index;
    float  pos;
    
    int     startFrame;
    int     currentFrame;
    int     endFrame;
    
    int     screenOffSet;
    int     screenIndex;
    int     port;
    bool    doSeek;
    bool    doFade;
    bool    fadeUp;
};

