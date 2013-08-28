//
//  WebHistoryVisualSystem.cpp
//

#include "WebHistoryVisualSystem.h"
#include "CloudsRGBDVideoPlayer.h"

#include <Poco/URI.h>

//--------------------------------------------------------------
//These methods let us add custom GUI parameters and respond to their events
void WebHistoryVisualSystem::selfSetupGui()
{

	customGui = new ofxUISuperCanvas("CUSTOM", gui);
	customGui->copyCanvasStyle(gui);
	customGui->copyCanvasProperties(gui);
	customGui->setName("Custom");
	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	customGui->addSlider("Custom Float 1", 1, 1000, &customFloat1);
	customGui->addSlider("Custom Float 2", 1, 1000, &customFloat2);
	customGui->addButton("Custom Button", false);
	customGui->addToggle("Custom Toggle", &customToggle);
	
	ofAddListener(customGui->newGUIEvent, this, &WebHistoryVisualSystem::selfGuiEvent);
	
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
}

void WebHistoryVisualSystem::selfGuiEvent(ofxUIEventArgs &e){
	if(e.widget->getName() == "Custom Button"){
		cout << "Button pressed!" << endl;
	}
}

//Use system gui for global or logical settings, for exmpl
void WebHistoryVisualSystem::selfSetupSystemGui(){
	
}

void WebHistoryVisualSystem::guiSystemEvent(ofxUIEventArgs &e){
	
}
//use render gui for display settings, like changing colors
void WebHistoryVisualSystem::selfSetupRenderGui(){

}

void WebHistoryVisualSystem::guiRenderEvent(ofxUIEventArgs &e){
	
}

//--------------------------------------------------------------
// selfSetup is called when the visual system is first instantiated
// This will be called during a "loading" screen, so any big images or
// geometry should be loaded here
void WebHistoryVisualSystem::selfSetup()
{    
    font.loadFont("GUI/NewMedia Fett.ttf", 12);
    
    string chromeHistoryPath = ofFilePath::getUserHomeDir() + "/Library/Application Support/Google/Chrome/Default/History";
    sqlite = new ofxSQLite(chromeHistoryPath);
	
	ofxSQLiteSelect sel = sqlite->select("url, last_visit_time").from("urls")
                                                                .order("last_visit_time", "DESC")
                                                                .execute().begin();
    
    int count = 0;
	while (sel.hasNext() && count < 50) {
        string url = sel.getString();
        time_t timestamp = sel.getInt();
        
        urls.push_back(url);
		sel.next();
        ++count;
        
        Poco::URI uri(url);
        string host = uri.getHost();
        vector<string> segments;
        uri.getPathSegments(segments);
        list<string> segmentList;
        for (int i = 0; i < segments.size(); i++) {
            segmentList.push_back(segments[i]);
        }
        
        try {
            HistoryNode * node = hosts.at(host);
            node->addChild(segmentList, 1);
        }
        catch (const std::out_of_range& e) {
            HistoryNode * node = new HistoryNode(host, timestamp, 0, segmentList);
            hosts[host] = node;
        }
	}
    
    for (map<string, HistoryNode *>::iterator it = hosts.begin(); it != hosts.end(); ++it) {
        it->second->print();
    }
    
    sel = sqlite->select("lower_term").from("keyword_search_terms")
                                      .order("url_id", "DESC")
                                      .execute().begin();
    
    count = 0;
	while (sel.hasNext() && count < 50) {
        string url = sel.getString();
        ofxTextWriter * term = new ofxTextWriter(url);
        searchTerms.push_back(term);
		sel.next();
        ++count;
	}
    
    currSearchTermIdx = 0;
    topSearchTermIdx = 0;
    searchTermCount = 1;
    
//	someImage.loadImage( getVisualSystemDataPath() + "images/someImage.png";
	
}

//--------------------------------------------------------------
// selfPresetLoaded is called whenever a new preset is triggered
// it'll be called right before selfBegin() and you may wish to
// refresh anything that a preset may offset, such as stored colors or particles
void WebHistoryVisualSystem::selfPresetLoaded(string presetPath)
{
	
}

//--------------------------------------------------------------
// selfBegin is called when the system is ready to be shown
// this is a good time to prepare for transitions
// but try to keep it light weight as to not cause stuttering
void WebHistoryVisualSystem::selfBegin()
{
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
}

//do things like ofRotate/ofTranslate here
//any type of transformation that doesn't have to do with the camera
void WebHistoryVisualSystem::selfSceneTransformation(){
	
}

//--------------------------------------------------------------
//normal update call
void WebHistoryVisualSystem::selfUpdate()
{

}

// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void WebHistoryVisualSystem::selfDraw()
{
    for (map<string, HistoryNode *>::iterator it = hosts.begin(); it != hosts.end(); ++it) {
        it->second->draw(font);
    }
}

// draw any debug stuff here
void WebHistoryVisualSystem::selfDrawDebug()
{
	
}

//--------------------------------------------------------------
// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
void WebHistoryVisualSystem::selfDrawBackground()
{
	//turn the background refresh off
	//bClearBackground = false;
    
    int stringHeight = 20;
    int maxNumStrings = ofGetHeight() / stringHeight;
    
    if (searchTerms[currSearchTermIdx]->isDone()) {
        // Start rendering the next term.
        currSearchTermIdx = (currSearchTermIdx + 1) % searchTerms.size();
        searchTerms[currSearchTermIdx]->reset();
        
        // Go to the next line.
        ++searchTermCount;
        if (searchTermCount > maxNumStrings) {
            searchTermCount = maxNumStrings;

            // Scroll up.
            topSearchTermIdx = (topSearchTermIdx + 1) % searchTerms.size();
        }
    }
    
    // Render the search terms.
    for (int i = 0; i < searchTermCount; i++) {
        int idx = (topSearchTermIdx + i) % searchTerms.size();
        searchTerms[idx]->update();
        font.drawString(searchTerms[idx]->textToRender(), 10, (i + 1) * stringHeight);
    }
}
// this is called when your system is no longer drawing.
// Right after this selfUpdate() and selfDraw() won't be called any more
void WebHistoryVisualSystem::selfEnd()
{
		
}
// this is called when you should clear all the memory and delet anything you made in setup
void WebHistoryVisualSystem::selfExit()
{

}

//events are called when the system is active
//Feel free to make things interactive for you, and for the user!
void WebHistoryVisualSystem::selfKeyPressed(ofKeyEventArgs & args){
	
}
void WebHistoryVisualSystem::selfKeyReleased(ofKeyEventArgs & args){
	
}

//--------------------------------------------------------------
void WebHistoryVisualSystem::selfMouseDragged(ofMouseEventArgs& data)
{

}

void WebHistoryVisualSystem::selfMouseMoved(ofMouseEventArgs& data){
	
}

//--------------------------------------------------------------
void WebHistoryVisualSystem::selfMousePressed(ofMouseEventArgs& data)
{

}

void WebHistoryVisualSystem::selfMouseReleased(ofMouseEventArgs& data){
	
}
