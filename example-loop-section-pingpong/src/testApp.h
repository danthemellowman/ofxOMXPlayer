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
	map<int, vector<ofxOMXPlayer*> > omxPlayers;
	vector<int> key;
	void onCharacterReceived(KeyListenerEventData& e);
	TerminalListener consoleListener;


    float  totalDuration;
    int  startTime;
    int  endTime;
    int  totalScreens;
    int  fade;
    int  index;
    int  filmIndex;
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
    bool    seeking;

    ofFbo ping;
    ofFbo pong;
};

