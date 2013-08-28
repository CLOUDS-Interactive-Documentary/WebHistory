//
//  HistoryNode.h
//  EmptyVisualSystem
//
//  Created by Elie Zananiri on 2013-07-28.
//
//

#pragma once

#include "ofMain.h"

extern int kLevelDepth;

//--------------------------------------------------------------
//--------------------------------------------------------------
class HistoryNode
{
public:
    HistoryNode(const string& name, int timestamp, int level, list<string>& childSegments, float latitude = ofRandom(360), float longitude = ofRandom(360));
    void addChild(list<string>& segments, int childLevel);
    
    void billboard();
    ofPoint draw(ofTrueTypeFont& font);
    void print();
    
private:
    string _name;
    int _timestamp;
    int _level;
    map<string, HistoryNode *> _children;
    
    float _latitude;
    float _longitude;
};
