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
	customGui = new ofxUISuperCanvas("WEB HISTORY", gui);
	customGui->copyCanvasStyle(gui);
	customGui->copyCanvasProperties(gui);
	customGui->setName("WEB_HISTORY");
	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
    customGui->addSlider("TYPE SPEED", 1, 100, &typeSpeed);
    customGui->addToggle("CLEAR SCREEN", &bClearScreen);
    
    customGui->addSpacer();
    customGui->addSlider("LINE HUE", 0.0, 1.0, HistoryNode::lineColor.getHue());
    customGui->addSlider("LINE SAT", 0.0, 1.0, HistoryNode::lineColor.getSaturation());
    customGui->addSlider("LINE BRI", 0.0, 1.0, HistoryNode::lineColor.getBrightness());
    customGui->addSlider("LINE ALPHA", 0.0, 1.0, HistoryNode::lineColor.a);
    
	customGui->addSpacer();
    customGui->addSlider("SPIN SPEED", 0, 5, &spinSpeed);
    customGui->addSlider("LEVEL DEPTH", 1, 100, &HistoryNode::levelDepth);
	customGui->addSlider("NOISE STEP", 0, 0.1, &HistoryNode::noiseStep);
	customGui->addSlider("NOISE AMOUNT", 1, 10, &HistoryNode::noiseAmount);
    
    customGui->addSpacer();
    customGui->addSlider("MIN Z", -2000.0, 0.0, &HistoryNode::minZ);
    customGui->addSlider("MAX Z", 0.0, 2000.0, &HistoryNode::maxZ);
    customGui->addSlider("MIN ALPHA", 0.0, 1.0, &HistoryNode::minAlpha);
    
    customGui->addSpacer();
    customGui->addSlider("LIST HUE", 0.0, 1.0, listColor.getHue());
    customGui->addSlider("LIST SAT", 0.0, 1.0, listColor.getSaturation());
    customGui->addSlider("LIST BRI", 0.0, 1.0, listColor.getBrightness());
    customGui->addSlider("LIST ALPHA", 0.0, 1.0, listColor.a);
    
    customGui->addSpacer();
    customGui->addSlider("NODE HUE", 0.0, 1.0, HistoryNode::textColor.getHue());
    customGui->addSlider("NODE SAT", 0.0, 1.0, HistoryNode::textColor.getSaturation());
    customGui->addSlider("NODE BRI", 0.0, 1.0, HistoryNode::textColor.getBrightness());
    customGui->addSlider("NODE ALPHA", 0.0, 1.0, HistoryNode::textColor.a);
    
    ofAddListener(customGui->newGUIEvent, this, &WebHistoryVisualSystem::selfGuiEvent);
	
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
}

//--------------------------------------------------------------
void WebHistoryVisualSystem::selfGuiEvent(ofxUIEventArgs &e)
{
    if (e.widget->getName() == "LIST HUE") {
        listColor.setHue(((ofxUISlider *)e.widget)->getValue() * 255);
	}
    else if (e.widget->getName() == "LIST SAT") {
        listColor.setSaturation(((ofxUISlider *)e.widget)->getValue() * 255);
	}
    else if (e.widget->getName() == "LIST BRI") {
        listColor.setBrightness(((ofxUISlider *)e.widget)->getValue() * 255);
	}
    else if (e.widget->getName() == "LIST ALPHA") {
        listColor.a = ((ofxUISlider *)e.widget)->getValue() * 255;
    }
    
    else if (e.widget->getName() == "NODE HUE") {
        HistoryNode::textColor.setHue(((ofxUISlider *)e.widget)->getValue() * 255);
	}
    else if (e.widget->getName() == "NODE SAT") {
        HistoryNode::textColor.setSaturation(((ofxUISlider *)e.widget)->getValue() * 255);
	}
    else if (e.widget->getName() == "NODE BRI") {
        HistoryNode::textColor.setBrightness(((ofxUISlider *)e.widget)->getValue() * 255);
	}
    else if (e.widget->getName() == "NODE ALPHA") {
        HistoryNode::textColor.a = ((ofxUISlider *)e.widget)->getValue() * 255;
    }
    
    else if (e.widget->getName() == "LINE HUE") {
        HistoryNode::lineColor.setHue(((ofxUISlider *)e.widget)->getValue() * 255);
	}
    else if (e.widget->getName() == "LINE SAT") {
        HistoryNode::lineColor.setSaturation(((ofxUISlider *)e.widget)->getValue() * 255);
	}
    else if (e.widget->getName() == "LINE BRI") {
        HistoryNode::lineColor.setBrightness(((ofxUISlider *)e.widget)->getValue() * 255);
	}
    else if (e.widget->getName() == "LINE ALPHA") {
        HistoryNode::lineColor.a = ((ofxUISlider *)e.widget)->getValue() * 255;
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
    // Load fonts.
    listColor.set(255, 128, 64);  // Pick a non-gray color so that HSB gets set properly.
    listFont.loadFont("Andale Mono.ttf", 12);
    HistoryNode::font.loadFont("Andale Mono.ttf", 12, true, true, true);
    
    // Set defaults.
    currSpin = 0.0f;
    spinSpeed = 0.5f;
    typeSpeed = 10;
    bClearScreen = false;
    
    if (fetchChromeHistory()) {
        ofLogNotice("VSWebHistory") << "Using live Chrome data" << endl;
    }
    else if (fetchSafariHistory()) {
        ofLogNotice("VSWebHistory") << "Using live Safari data" << endl;
    }
    else if (fetchFirefoxHistory()) {
        ofLogNotice("VSWebHistory") << "Using live Firefox data" << endl;
    }
    else if (fetchChromeHistory(true)) {
        ofLogNotice("VSWebHistory") << "Using sample Chrome data" << endl;
    }
    else {
        ofLogError("VSWebHistory") << "No available web history!" << endl;
    }
}

//--------------------------------------------------------------
bool WebHistoryVisualSystem::fetchChromeHistory(bool bUseSample)
{
    string chromeHistoryPath;
    if (bUseSample) {
        chromeHistoryPath = getVisualSystemDataPath() + "SampleChromeHistory";
    }
    else {
        chromeHistoryPath = ofFilePath::getUserHomeDir() + "/Library/Application Support/Google/Chrome/Default/History";
    }
    
    ofxSQLite sqlite;
    if (!sqlite.setup(chromeHistoryPath)) {
        // No dice :(
        return false;
    }
    	
	ofxSQLiteSelect sel = sqlite.select("url, last_visit_time").from("urls")
                                .order("last_visit_time", "DESC")
                                .execute().begin();
    
    if (!sel.hasRow()) {
        // No rows, db is probably locked.
        return false;
    }
    
    int count = 0;
	while (sel.hasNext() && count < 100) {
        string url = sel.getString();
        time_t timestamp = sel.getInt();
        
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
        
    if (ofGetLogLevel() <= OF_LOG_VERBOSE) {
        for (auto& it : hosts) {
            it.second->print();
        }
    }
    
    sel = sqlite.select("lower_term").from("keyword_search_terms")
                .order("url_id", "DESC")
                .execute().begin();
    
    count = 0;
	while (sel.hasNext() && count < 100) {
        string url = sel.getString();
        ofxTextWriter * term = new ofxTextWriter(url);
        searchTerms.push_back(term);
		sel.next();
        ++count;
	}
    
    currSearchTermIdx = 0;
    topSearchTermIdx = 0;
    searchTermCount = 1;
    
    return true;
}

//--------------------------------------------------------------
bool WebHistoryVisualSystem::fetchSafariHistory()
{
    string safariHistoryPath = ofFilePath::getUserHomeDir() + "/Library/Safari/History.plist";
    
    ofxXmlSettings xml;
    if (!xml.loadFile(safariHistoryPath)) {
        // No dice :(
        return false;
    }
    
    return true;
}

//--------------------------------------------------------------
bool WebHistoryVisualSystem::fetchFirefoxHistory()
{
    ofDirectory dir;
    dir.listDir(ofFilePath::getUserHomeDir() + "/Library/Application Support/Firefox/Profiles/");
    if (!dir.size()) {
        // No dice :(
        return false;
    }
    
    string firefoxHistoryPath = dir.getPath(0) + "/places.sqlite";
    
    ofxSQLite sqlite;
    sqlite.setup(firefoxHistoryPath);
    
	ofxSQLiteSelect sel = sqlite.select("url, last_visit_date").from("moz_places")
                                .order("last_visit_date", "DESC")
                                .execute().begin();
    
    if (!sel.hasRow()) {
        // No rows, db is probably locked.
        return false;
    }
    
    int count = 0;
	while (sel.hasNext() && count < 50) {
        string url = sel.getString();
        time_t timestamp = sel.getInt();
        
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
    
    if (ofGetLogLevel() <= OF_LOG_VERBOSE) {
        for (auto& it : hosts) {
            it.second->print();
        }
    }
    
    return true;
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

}

//do things like ofRotate/ofTranslate here
//any type of transformation that doesn't have to do with the camera
void WebHistoryVisualSystem::selfSceneTransformation(){
	
}

//--------------------------------------------------------------
//normal update call
void WebHistoryVisualSystem::selfUpdate()
{
    currSpin += spinSpeed;
}

// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void WebHistoryVisualSystem::selfDraw()
{
    ofRotate(currSpin, 0, 1, 0);
    for (auto& it: hosts) {
        it.second->draw();
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
    ofSetColor(listColor);
    
    int stringHeight = 20;
    int maxNumStrings = ofGetHeight() / stringHeight;
    
    if (bClearScreen) {
        if (searchTermCount > 0 && (ofGetFrameNum() % (int)typeSpeed == 0)) {
            // Scroll up.
            topSearchTermIdx = (topSearchTermIdx + 1) % searchTerms.size();
            --searchTermCount;
        }
    }
    else {
        if (searchTerms[currSearchTermIdx]->isDone()) {
            // Start rendering the next term.
            currSearchTermIdx = (currSearchTermIdx + 1) % searchTerms.size();
            searchTerms[currSearchTermIdx]->reset(typeSpeed);
            
            // Go to the next line.
            ++searchTermCount;
            if (searchTermCount > maxNumStrings) {
                searchTermCount = maxNumStrings;

                // Scroll up.
                topSearchTermIdx = (topSearchTermIdx + 1) % searchTerms.size();
            }
        }
    }
    
    // Render the search terms.
    for (int i = 0; i < searchTermCount; i++) {
        int idx = (topSearchTermIdx + i) % searchTerms.size();
        searchTerms[idx]->update();
        listFont.drawString(searchTerms[idx]->textToRender(), 10, (i + 1) * stringHeight);
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
