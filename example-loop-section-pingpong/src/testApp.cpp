#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
	string videoPath = ofToDataPath("../../../video/video.mp4", true);


	for(int i = 0; i < 2; i++){
		ofxOMXPlayerSettings settings;
		settings.videoPath = videoPath;
		settings.useHDMIForAudio	= true;		//default true
		settings.enableTexture		= true;		//default true
		settings.enableLooping		= true;		//default true
		settings.enableAudio		= true;		//default true, save resources by disabling
		//settings.doFlipTexture = true;		//default false
		if (!settings.enableTexture) 
		{
			/*
			 We have the option to pass in a rectangle
			 to be used for a non-textured player to use (as opposed to the default full screen)
			 */
			settings.displayRect.width = 480;
			settings.displayRect.height = 300;
			settings.displayRect.x = 480;
			settings.displayRect.y = 300;
		}
		//so either pass in the settings
		ofxOMXPlayer* player = new ofxOMXPlayer();
		player->setup(settings);
		omxPlayers.push_back(player);
		//if(i!=0){
			omxPlayers[i]->setPaused(true);
		//}

	}
    index = 0;
	totalScreens = 200;
	screenIndex = 0;
	fade = 255;
	doSeek = true;
    doFade = true;
    fadeUp = true;
    consoleListener.setup(this);
}



//--------------------------------------------------------------
void testApp::update()
{

	totalDuration = (omxPlayers[index]->getTotalNumFrames())/totalScreens;
    startFrame = totalDuration*(screenIndex);
    endFrame = startFrame+(totalDuration);
    startTime = startFrame/30.0;
    endTime = endFrame/30.0;

    for(int i = 0; i < omxPlayers.size(); i++){
    	if(doSeek){
    		if(omxPlayers[i]->isPaused()){
    			omxPlayers[i]->setPaused(false);
    		}
    		omxPlayers[i]->setSeek(startTime);
    		doSeek = false;
    	}else{
	    	if(index == i){
	    		if(omxPlayers[i]->isPaused()){
	    			omxPlayers[i]->setPaused(false);
	    		}
	    	}else{
	    		if((omxPlayers[i]->getCurrentFrame()) >= endFrame){
	    			if(omxPlayers[i]->isPaused()){
	    				omxPlayers[i]->setPaused(false);
	    			}
	    			omxPlayers[i]->setSeek(startTime);
	    		}
	    		if(!omxPlayers[i]->isPaused()){
	    			omxPlayers[i]->setPaused(true);
	    		}
	    	}
	    }
    }

    if((omxPlayers[index]->getCurrentFrame()+startFrame) >= endFrame){
    	index++;
    	if(index >= omxPlayers.size()){
    		index = 0;
    	}
    	doFade = true;
    	if(fade == 0){
    		fadeUp = true;
    	}else{
    		fadeUp = false;
    	}
    }

	if(doFade){
		if(!fadeUp)
		{
			if(fade > 200)
			{
				fade -=25;
			}
			else
			{
				fade-=10;
			}
			if(fade <= 0)
			{
				doFade = false;
				fade = 0;
			}
		}else{
			if(fade < 200)
			{
				fade +=25;
			}
			else
			{
				fade+=10;
			}
			if(fade >= 255)
			{
				doFade = false;
				fade = 255;
			}
		}
	}
}


//--------------------------------------------------------------
void testApp::draw()
{
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, fade);
	omxPlayers[0]->draw(0, 0, ofGetWidth(), ofGetHeight());
	ofDrawBitmapStringHighlight("startFrame: "+ofToString(startFrame)+" endFrame: "+ofToString(endFrame), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
	ofDrawBitmapStringHighlight("current Frame (calc) "+ofToString((startFrame+omxPlayers[0]->getCurrentFrame())), 60, 90, ofColor(ofColor::black, 90), ofColor::yellow);


	ofSetColor(255, 255, 255, 255-fade);
	omxPlayers[1]->draw(0, 0, ofGetWidth(), ofGetHeight());
	ofDrawBitmapStringHighlight("startFrame: "+ofToString(startFrame)+" endFrame: "+ofToString(endFrame), 60, 120, ofColor(ofColor::black, 90), ofColor::yellow);
	ofDrawBitmapStringHighlight("current Frame (calc) "+ofToString((startFrame+omxPlayers[1]->getCurrentFrame())), 60, 150, ofColor(ofColor::black, 90), ofColor::yellow);
	ofDisableAlphaBlending();
}

void testApp::keyPressed(int key)
{
    ofLog(OF_LOG_VERBOSE, "%c keyPressed", key);
	if (key == 'p') 
	{
		index++;
    	if(index >= omxPlayers.size()){
    		index = 0;
    	}
    	doFade = true;
    	if(fade == 0){
    		fadeUp = true;
    	}else{
    		fadeUp = false;
    	}
	}
	// if(key == '=')
	// {
	// 	screenIndex+=1;
	// 	if(screenIndex >= totalScreens)
	// 	{
	// 		screenIndex = 0;
	// 	}
	// 	startFrame = totalDuration*(screenIndex);
 //        	endFrame = startFrame+(totalDuration);
 //        	startTime = startFrame/30.0;
 //        	endTime = endFrame/30.0;
	// 	doSeek = true;
	// }

}

void testApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}
