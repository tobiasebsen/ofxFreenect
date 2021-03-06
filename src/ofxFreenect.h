//
//  ofxFreenect.h
//
//  Created by Tobias Ebsen on 26/03/14.
//
//

#pragma once

#include "ofMain.h"
#include "libfreenect.h"

#if defined(_MSC_VER) || defined(_WIN32) || defined(WIN32) || defined(__MINGW32__)
#else
#include <libusb.h>
#endif

#define OFX_FREENECT_CMD_START_VIDEO     1
#define OFX_FREENECT_CMD_START_DEPTH     2
#define OFX_FREENECT_CMD_STOP_VIDEO      3
#define OFX_FREENECT_CMD_STOP_DEPTH      4
#define OFX_FREENECT_CMD_WAIT            5
#define OFX_FREENECT_CMD_REOPEN          6


// FREENECT CONTEXT
class ofxFreenectDepthTable;

class ofxFreenectContext {
public:
    ofxFreenectContext();

    void init();
    
    bool isInitialized();
    int numDevices();
    
    freenect_context *getContext();

private:
    freenect_context *f_ctx;
    bool bInited;
};


// FREENECT DEVICE
class ofxFreenectDevice : public ofThread {
public:
    ofxFreenectDevice();
    ~ofxFreenectDevice();

    void open();
    void close();
    void update();
    
    void draw(float x, float y);
    void draw(float x, float y, float w, float h);
    void drawDepth(float x, float y);
    void drawDepth(float x, float y, float w, float h);
    
    void applyFlag(freenect_flag flag, freenect_flag_value value);
    void applyCommand(int command);
    
    // Accessors
    int getWidth();
    int getHeight();
    ofPixels & getPixels();
    ofShortPixels & getDepthPixels();
    ofTexture & getTextureReference();
    ofTexture & getTextureReferenceDepth();
    bool isOpen();
    bool isFrameNew();
    bool isFrameNewVideo();
    bool isFrameNewDepth();
    int numDevices();
    freenect_device *getDevice();

private:
    static void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp);
    static void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp);
    
    void threadedFunction();

    freenect_context *f_ctx;
    freenect_device *f_dev;
    
    freenect_frame_mode vmode;
    freenect_frame_mode dmode;
    map<freenect_flag,freenect_flag_value> pendingFlags;
    vector<int> pendingCommands;
    
    bool bIsOpen, bWasDisconnected;
	bool bIsFrameNewVideo, bIsFrameNewDepth;
	bool bNeedsUpdateVideo, bNeedsUpdateDepth;
    
    ofTexture videoTexture;
    ofTexture depthTexture;
    
    ofPixels        videoPixels, videoPixelsBack;
    ofShortPixels   depthPixels, depthPixelsBack;
    
    ofxFreenectDepthTable* depthTable;
};

// DEPTH TABLE
class ofxFreenectDepthTable {
public:
    void apply(uint16_t* pixels, int count);
    
    void generateLinear();
    void generateExponential(float power = 3, float multiply = 6, bool inverse = false);

private:
    uint16_t table[2048];
};