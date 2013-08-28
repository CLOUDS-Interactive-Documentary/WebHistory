//
//  HistoryNode.cpp
//  EmptyVisualSystem
//
//  Created by Elie Zananiri on 2013-07-28.
//
//

#include "HistoryNode.h"

int kLevelDepth = 100;

//--------------------------------------------------------------
HistoryNode::HistoryNode(const string& segment, int timestamp, int level, list<string>& childSegments, float latitude, float longitude)
: _name(segment)
, _timestamp(timestamp)
, _level(level)
, _latitude(latitude)
, _longitude(longitude)
{
    addChild(childSegments, level + 1);
}

//--------------------------------------------------------------
void HistoryNode::addChild(list<string>& segments, int childLevel)
{
    if (segments.size() > 0) {
        string nextSegment = segments.front();
        segments.pop_front();
        try {
            HistoryNode * nextChild = _children.at(nextSegment);
            nextChild->addChild(segments, childLevel);
        }
        catch (const std::out_of_range& e) {
            HistoryNode * nextChild = new HistoryNode(nextSegment, _timestamp, childLevel, segments, _latitude + ofRandom(-10, 10), _longitude + ofRandom(-10, 10));
            _children[nextSegment] = nextChild;
        }
    }
}

//--------------------------------------------------------------
ofPoint HistoryNode::draw(ofTrueTypeFont& font)
{
    ofQuaternion latRot, longRot, spinQuat;
    latRot.makeRotate(_latitude, 1, 0, 0);
    longRot.makeRotate(_longitude, 0, 1, 0);
//    spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);
    
    ofVec3f center = ofVec3f(0, 0, kLevelDepth * (_level + 1));
    ofVec3f worldPoint = latRot * longRot * spinQuat * center;
    
    ofPushMatrix();
    ofTranslate(worldPoint);
    {
        ofSetColor(ofColor::red);
        ofSphere(ofVec3f(), 5);
    
        ofPushMatrix();
        {
            billboard();
            ofSetColor(ofColor::white);
//            ofDrawBitmapString(_name, worldPoint);
            ofScale(1, -1, 1);
            font.drawString(_name, 0, 0);
        }
        ofPopMatrix();
    }
    ofPopMatrix();
    
    for (auto& it : _children) {
        ofPoint childPoint = it.second->draw(font);
        
        ofSetColor(ofColor::grey);
        ofLine(worldPoint, childPoint);
    }
    
    return worldPoint;
}

//--------------------------------------------------------------
void HistoryNode::billboard()
{
	// get the current modelview matrix
	float modelview[16];    
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    
	// undo all rotations
	// beware all scaling is lost as well
	for (int i = 0; i < 3; i++) {
		for (int j=0; j < 3; j++) {
			if (i == j) {
				modelview[i * 4 + j] = 1.0;
            }
			else {
				modelview[i * 4 + j] = 0.0;
            }
		}
    }
    
	// set the modelview with no rotations
	glLoadMatrixf(modelview);
}

//--------------------------------------------------------------
void HistoryNode::print()
{
    for (int i = 0; i < _level; i++) {
        cout << "  ";
    }
    cout << _name << endl;
    
    for (auto& it : _children) {
        it.second->print();
    }
}
