#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	// ofSetVerticalSync(true);
	// ofSetFrameRate(60.0);
	ofSetLogLevel(OF_LOG_SILENT);
	// ofSetLogLevel("ofThread", OF_LOG_ERROR);
	ofBackground(0, 0, 0);
	ofHideCursor();
	string videoPath;
	//omxPlayers.resize(2);
	//for(int h = 0; h < omxPlayers.size(); h++){
	for(int i = 0; i < 4 ; i++){
		//if(i%2==0){
			videoPath = ofToDataPath("../../../video/video.mp4", true);
		//}else{
		//	videoPath = ofToDataPath("../../../video/Timecoded_Big_bunny_1.mov", true);
		//}
		ofxOMXPlayerSettings settings;
		settings.videoPath = videoPath;
		settings.useHDMIForAudio	= true;		//default true
		settings.enableTexture		= true;		//default true
		settings.enableLooping		= true;		//default true
		settings.enableAudio		= false;		//default true, save resources by disabling
		// if (!settings.enableTexture) 
		// {
		// 	settings.displayRect.width = 480;
		// 	settings.displayRect.height = 300;
		// 	settings.displayRect.x = 0;
		// 	settings.displayRect.y = 0;
		// }
		ofxOMXPlayer* player = new ofxOMXPlayer();
		player->setup(settings);
		omxPlayers.push_back(player);
		omxPlayers[i]->setPaused(true);
	}
	startTime.resize(omxPlayers.size());
	endTime.resize(omxPlayers.size());
	startFrame.resize(omxPlayers.size());
	endFrame.resize(omxPlayers.size());
	//}
	filmIndex = 0;
	nextIndex = 1;
	totalScreens = 200;
	screenIndex = 0;
	fade = 0;
	doSeek = true;
    doFade = true;
    fadeUp = false;
    newMovie = false;
    consoleListener.setup(this);


    ping.allocate(480, 272, GL_RGBA);
    pong.allocate(480, 272, GL_RGBA);

    ping.begin();
    ofClear(0, 0, 0, 0);
    ping.end();

    pong.begin();
    ofClear(0, 0, 0, 0);
    pong.end();
}



//--------------------------------------------------------------
void testApp::update()
{
	if(doSeek){
		for(int i = 0; i < omxPlayers.size(); i++){
			totalDuration = (omxPlayers[i]->getTotalNumFrames())/totalScreens;
			startFrame[i] = totalDuration*(i);
			endFrame[i] = startFrame[i]+(totalDuration);
			startTime[i] = startFrame[i]/30.0;
			endTime[i] = endFrame[i]/30.0;
    		if(omxPlayers[i]->isPaused()){
    			omxPlayers[i]->setPaused(false);
  			}
    		omxPlayers[i]->seekToPosition(startTime[i]);
    		doFade = true;
    		fadeUp = false;
	    }	
	    doSeek = false;
	}else{

		if(omxPlayers[filmIndex]->getCurrentFrame() >= endFrame[filmIndex]-60 && !doFade){
	    	doFade = true;
	    	fadeUp = false;
	    }
	    if(omxPlayers[filmIndex]->getCurrentFrame() >= endFrame[filmIndex]){
	    	if(omxPlayers[filmIndex]->isFrameNew()){
				omxPlayers[filmIndex]->seekToPosition(startTime[filmIndex]);
			}
	    }

	    for(int i = 0; i < omxPlayers.size(); i++){
	    	if(filmIndex == i){
	    		if(omxPlayers[i]->isPaused()){
	    			omxPlayers[i]->setPaused(false);
	    		}
	    	}else{
	    		if(!omxPlayers[i]->isPaused()){
	    			omxPlayers[i]->setPaused(true);
	    		}
	    	}
	    }
	}
	
	if(doFade){
		if(!fadeUp)
		{
			fade-=255/100;
			if(fade <= 0)
			{
				fadeUp = true;
				fade = 0;
			}
		}else{
			fade+=255/100;
			if(fade >= 255)
			{
				doFade = false;
				fade = 255;
			}
		}
	}else{
		
		fade = 255;

	}

	if(newMovie){
		if(filmIndex != nextIndex){
			if(fade == 0){
				filmIndex = nextIndex;
			}
		}else{
			newMovie = false;
		}
	}
}


//--------------------------------------------------------------
void testApp::draw()
{
	ofEnableAlphaBlending();
	if(omxPlayers[filmIndex]->isFrameNew()){
		ofPushStyle();
		ping.begin();
		ofClear(0, 0, 0, 0);
		ofSetColor(255, 255, 255, fade);
		omxPlayers[filmIndex]->draw(0, 0, 480, 280);
		ping.end();
		ofPopStyle();
	}

	if(omxPlayers[pfilmIndex]->isFrameNew()){
		ofPushStyle();
		pong.begin();
		ofClear(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255-fade);
		omxPlayers[pfilmIndex]->draw(0, 0, 480, 280);
		pong.end();
		ofPopStyle();
	}


	ofPushStyle();
	ofSetColor(255, 255, 255, fade);
	ping.draw(0, 0);
	ofPopStyle();

	ofPushStyle();
	ofSetColor(255, 255, 255, 255-fade);
	ping.draw(0, 0);
	ofPopStyle();

	ofDrawBitmapStringHighlight("film index:"+ofToString(filmIndex)+" doFade: "+ofToString(doFade)+" fadeUp: "+ofToString(fadeUp), 60, 30, ofColor(ofColor::black, 90), ofColor::yellow);
	ofDrawBitmapStringHighlight("startFrame: "+ofToString(startFrame[filmIndex])+" endFrame: "+ofToString(endFrame[filmIndex]), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
	ofDrawBitmapStringHighlight("current Frame "+ofToString(omxPlayers[filmIndex]->getCurrentFrame()), 60, 90, ofColor(ofColor::black, 90), ofColor::yellow);
	ofDisableAlphaBlending();
}

void testApp::keyPressed(int key)
{
	if (key == '=') 
	{
		pfilmIndex = filmIndex;
		nextIndex = filmIndex+1;
    	if(nextIndex >= omxPlayers.size()){
    		nextIndex = 0;
    	}
    	newMovie = true;
    	doFade = true;
    	fadeUp = false;

    }
    if (key == '-') 
	{
		pfilmIndex = filmIndex;
    	nextIndex = filmIndex-1;
    	if(nextIndex <=0){
    		nextIndex = omxPlayers.size()-1;
    	}
    	newMovie = true;
    	doFade = true;
    	fadeUp = false;
	}
}

void testApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}
