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
	vector<int> key;
	void onCharacterReceived(KeyListenerEventData& e);
	TerminalListener consoleListener;


    float  totalDuration;
    vector<int> startTime;
    vector<int>  endTime;
    int  totalScreens;
    int  fade;
    int  index;
    int  filmIndex;
    float  pos;
    
    vector<int>     startFrame;
    vector<int>     endFrame;
    int     pfilmIndex;
    int     nextIndex;
    int     screenOffSet;
    int     screenIndex;
    int     port;
    bool    doSeek;
    bool    doFade;
    bool    doLoopFade;
    bool    fadeUp;
    bool    seeking;
    bool    newMovie;
    ofFbo ping;
    ofFbo pong;
};

