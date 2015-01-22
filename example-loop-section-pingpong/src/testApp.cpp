#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	// ofSetVerticalSync(true);
	// ofSetFrameRate(60.0);
	ofSetLogLevel(OF_LOG_SILENT);
	// ofSetLogLevel("ofThread", OF_LOG_ERROR);

	isServer = false;

	ofxXmlSettings XML;
    bool loadedFile = XML.loadFile("settings.xml");
    
    if(loadedFile){
    	// string systemCall = XML.getValue("settings:smbString", "");
    	// if(systemCall != ""){
    	// 	system(systemCall.c_str());
    	// }
    	isServer = XML.getValue("settings:isServer", 0)==1?true:false;
    	totalScreens = XML.getValue("settings:totalScreens", 20);
	   if(!isServer){
	       screenIndex = XML.getValue("settings:screenIndex", 0);
	       port = XML.getValue("settings:client:port", 7778);
	       client = new ofxClientOSCManager();
	       client->init(screenIndex, port);
	       
	       commonTimeOsc = client->getCommonTimeOscObj();
	       commonTimeOsc->setEaseOffset( true );
	       
	       ofAddListener(client->newDataEvent, this, &testApp::newData);
	   }else{
	       server = new ofxServerOscManager();
	       port = XML.getValue("settings:client:port", 7778);
	       int serverport = XML.getValue("settings:server:port", 7777);
	       string ipAddr = XML.getValue("settings:server:address", "192.168.2.255");
	       server->init(ipAddr, port, serverport);
	       screenIndex = XML.getValue("settings:screenIndex", 0);
	   }
	}else{
		if(!isServer){
	       screenIndex = 0;
	       port = 7778;
	       client = new ofxClientOSCManager();
	       client->init( screenIndex, port);
	       
	       commonTimeOsc = client->getCommonTimeOscObj();
	       commonTimeOsc->setEaseOffset( true );
	       
	       ofAddListener(client->newDataEvent, this, &testApp::newData);
	   }else{
	       server = new ofxServerOscManager();
	       server->init("127.0.0.1", 7778, 7777);
	   }
	}
	ofBackground(0, 0, 0);
	ofHideCursor();
	string videoPath;
	for(int i = 0; i < 5; i++){
		videoPath = ofToDataPath("../../../video/video"+ofToString(i)+".mp4", true);
		ofxOMXPlayerSettings settings;
		settings.videoPath = videoPath;
		settings.useHDMIForAudio	= true;		//default true
		settings.enableTexture		= true;		//default true
		settings.enableLooping		= true;		//default true
		settings.enableAudio		= false;		//default true, save resources by disabling
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
	fade = 0;
	doSeek = true;
    doFade = true;
    fadeUp = false;
    newMovie = false;
    consoleListener.setup(this);


    ping.allocate(480, 272, GL_RGBA);

    ping.begin();
    ofClear(0, 0, 0, 0);
    ping.end();

    debug = false;
}



//--------------------------------------------------------------
void testApp::update()
{
	if(doSeek){
		for(int i = 0; i < omxPlayers.size(); i++){
			totalDuration = (omxPlayers[i]->getTotalNumFrames())/totalScreens;
			startFrame[i] = totalDuration*(screenIndex);
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
	    	if(isServer){
	    		doFade = true;
	    		DataPacket p;
	    		p.valuesString.push_back("nextVideo");
	    		server->sendData(p);
	    		keyPressed('=');
	    	}
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
			fade+=255/60;
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
		ofPushMatrix();
		ofScale(ping.getWidth()/omxPlayers[filmIndex]->getWidth(), ping.getHeight()/omxPlayers[filmIndex]->getHeight());
		omxPlayers[filmIndex]->draw(0, 0, omxPlayers[filmIndex]->getWidth(), omxPlayers[filmIndex]->getHeight());
		ofPopMatrix();
		ping.end();
		ofPopStyle();
	}

	// if(omxPlayers[pfilmIndex]->isFrameNew()){
	// 	ofPushStyle();
	// 	pong.begin();
	// 	ofClear(0, 0, 0, 0);
	// 	ofSetColor(255, 255, 255, 255-fade);
	// 	omxPlayers[pfilmIndex]->draw(0, 0, 480, 280);
	// 	pong.end();
	// 	ofPopStyle();
	// }

	//if(filmIndex == nextIndex){
	ofPushStyle();
	ofPushMatrix();
	ofScale((float)ofGetWindowWidth()/ping.getWidth(), (float)ofGetWindowHeight()/ping.getHeight());
	ofSetColor(255, 255, 255, fade);
	ping.draw(0, 0, 480, 272);
	ofPopMatrix();
	ofPopStyle();
	//}

	// if(nextIndex!=filmIndex && fade > 0){
	// 	ofPushStyle();
	// 	ofSetColor(255, 255, 255, 255-fade);
	// 	pong.draw(0, 0);
	// 	ofPopStyle();
	// }

	if(debug){
		ofDrawBitmapStringHighlight("film index:"+ofToString(filmIndex)+" doFade: "+ofToString(doFade)+" fadeUp: "+ofToString(fadeUp), 60, 30, ofColor(ofColor::black, 90), ofColor::yellow);
		ofDrawBitmapStringHighlight("startFrame: "+ofToString(startFrame[filmIndex])+" endFrame: "+ofToString(endFrame[filmIndex]), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
		ofDrawBitmapStringHighlight("current Frame "+ofToString(omxPlayers[filmIndex]->getCurrentFrame()), 60, 90, ofColor(ofColor::black, 90), ofColor::yellow);
		ofDisableAlphaBlending();
	}
}

void testApp::newData( DataPacket& _packet  ){
  	keyPressed('=');
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

    if(key == 'd'){
    	debug = !debug;
    }
}

void testApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}
