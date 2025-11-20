#pragma once
#include <cstdint>
#include "dshow/dshow.h"
#include "dshow/dvdmedia.h"
#include "dshow/bdatypes.h"
#include <map>
#include <vector>
#include <string>

struct ICaptureGraphBuilder2;
struct IGraphBuilder;
struct IBaseFilter;
struct IAMCrossbar;
struct IMediaControl;
struct ISampleGrabber;
struct IMediaEventEx;
struct IAMStreamConfig;
struct _AMMediaType;
class SampleGrabberCallback;

//videoInput defines
#define VI_VERSION      0.1995
#define VI_MAX_CAMERAS  20
#define VI_NUM_TYPES    20 //MGB
#define VI_NUM_FORMATS  18 //DON'T TOUCH

struct CameraSupportSize
{
    uint32_t width;
    uint32_t height;
    CameraSupportSize(uint32_t w, uint32_t h) :width(w), height(h) {}
};
using  SupportSizeVector = std::vector<CameraSupportSize>;

namespace xreal_dshow {

int cvRound( double value );

enum
{
    // modes of the controlling registers (can be: auto, manual, auto single push, absolute Latter allowed with any other mode)
    // every feature can have only one mode turned on at a time
    CV_CAP_PROP_DC1394_OFF         = -4,  //turn the feature off (not controlled manually nor automatically)
    CV_CAP_PROP_DC1394_MODE_MANUAL = -3, //set automatically when a value of the feature is set by the user
    CV_CAP_PROP_DC1394_MODE_AUTO = -2,
    CV_CAP_PROP_DC1394_MODE_ONE_PUSH_AUTO = -1,
    CV_CAP_PROP_POS_MSEC       =0,
    CV_CAP_PROP_POS_FRAMES     =1,
    CV_CAP_PROP_POS_AVI_RATIO  =2,
    CV_CAP_PROP_FRAME_WIDTH    =3,
    CV_CAP_PROP_FRAME_HEIGHT   =4,
    CV_CAP_PROP_FPS            =5,
    CV_CAP_PROP_FOURCC         =6,
    CV_CAP_PROP_FRAME_COUNT    =7,
    CV_CAP_PROP_FORMAT         =8,
    CV_CAP_PROP_MODE           =9,
    CV_CAP_PROP_BRIGHTNESS    =10,
    CV_CAP_PROP_CONTRAST      =11,
    CV_CAP_PROP_SATURATION    =12,
    CV_CAP_PROP_HUE           =13,
    CV_CAP_PROP_GAIN          =14,
    CV_CAP_PROP_EXPOSURE      =15,
    CV_CAP_PROP_CONVERT_RGB   =16,
    CV_CAP_PROP_WHITE_BALANCE_BLUE_U =17,
    CV_CAP_PROP_RECTIFICATION =18,
    CV_CAP_PROP_MONOCROME     =19,
    CV_CAP_PROP_SHARPNESS     =20,
    CV_CAP_PROP_AUTO_EXPOSURE =21, // exposure control done by camera,
                                   // user can adjust refernce level
                                   // using this feature
    CV_CAP_PROP_GAMMA         =22,
    CV_CAP_PROP_TEMPERATURE   =23,
    CV_CAP_PROP_TRIGGER       =24,
    CV_CAP_PROP_TRIGGER_DELAY =25,
    CV_CAP_PROP_WHITE_BALANCE_RED_V =26,
    CV_CAP_PROP_ZOOM          =27,
    CV_CAP_PROP_FOCUS         =28,
    CV_CAP_PROP_GUID          =29,
    CV_CAP_PROP_ISO_SPEED     =30,
    CV_CAP_PROP_MAX_DC1394    =31,
    CV_CAP_PROP_BACKLIGHT     =32,
    CV_CAP_PROP_PAN           =33,
    CV_CAP_PROP_TILT          =34,
    CV_CAP_PROP_ROLL          =35,
    CV_CAP_PROP_IRIS          =36,
    CV_CAP_PROP_SETTINGS      =37,

    CV_CAP_PROP_AUTOGRAB      =1024, // property for highgui class CvCapture_Android only
    CV_CAP_PROP_SUPPORTED_PREVIEW_SIZES_STRING=1025, // readonly, tricky property, returns cpnst char* indeed
    CV_CAP_PROP_PREVIEW_FORMAT=1026, // readonly, tricky property, returns cpnst char* indeed

    // OpenNI map generators
    CV_CAP_OPENNI_DEPTH_GENERATOR = 1 << 31,
    CV_CAP_OPENNI_IMAGE_GENERATOR = 1 << 30,
    CV_CAP_OPENNI_GENERATORS_MASK = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_OPENNI_IMAGE_GENERATOR,

    // Properties of cameras available through OpenNI interfaces
    CV_CAP_PROP_OPENNI_OUTPUT_MODE     = 100,
    CV_CAP_PROP_OPENNI_FRAME_MAX_DEPTH = 101, // in mm
    CV_CAP_PROP_OPENNI_BASELINE        = 102, // in mm
    CV_CAP_PROP_OPENNI_FOCAL_LENGTH    = 103, // in pixels
    CV_CAP_PROP_OPENNI_REGISTRATION    = 104, // flag
    CV_CAP_PROP_OPENNI_REGISTRATION_ON = CV_CAP_PROP_OPENNI_REGISTRATION, // flag that synchronizes the remapping depth map to image map
                                                                          // by changing depth generator's view point (if the flag is "on") or
                                                                          // sets this view point to its normal one (if the flag is "off").
    CV_CAP_PROP_OPENNI_APPROX_FRAME_SYNC = 105,
    CV_CAP_PROP_OPENNI_MAX_BUFFER_SIZE   = 106,
    CV_CAP_PROP_OPENNI_CIRCLE_BUFFER     = 107,
    CV_CAP_PROP_OPENNI_MAX_TIME_DURATION = 108,

    CV_CAP_PROP_OPENNI_GENERATOR_PRESENT = 109,

    CV_CAP_OPENNI_IMAGE_GENERATOR_PRESENT         = CV_CAP_OPENNI_IMAGE_GENERATOR + CV_CAP_PROP_OPENNI_GENERATOR_PRESENT,
    CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE     = CV_CAP_OPENNI_IMAGE_GENERATOR + CV_CAP_PROP_OPENNI_OUTPUT_MODE,
    CV_CAP_OPENNI_DEPTH_GENERATOR_BASELINE        = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_BASELINE,
    CV_CAP_OPENNI_DEPTH_GENERATOR_FOCAL_LENGTH    = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_FOCAL_LENGTH,
    CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION    = CV_CAP_OPENNI_DEPTH_GENERATOR + CV_CAP_PROP_OPENNI_REGISTRATION,
    CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION_ON = CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION,

    // Properties of cameras available through GStreamer interface
    CV_CAP_GSTREAMER_QUEUE_LENGTH   = 200, // default is 1
    CV_CAP_PROP_PVAPI_MULTICASTIP   = 300, // ip for anable multicast master mode. 0 for disable multicast

    // Properties of cameras available through XIMEA SDK interface
    CV_CAP_PROP_XI_DOWNSAMPLING  = 400,      // Change image resolution by binning or skipping.
    CV_CAP_PROP_XI_DATA_FORMAT   = 401,       // Output data format.
    CV_CAP_PROP_XI_OFFSET_X      = 402,      // Horizontal offset from the origin to the area of interest (in pixels).
    CV_CAP_PROP_XI_OFFSET_Y      = 403,      // Vertical offset from the origin to the area of interest (in pixels).
    CV_CAP_PROP_XI_TRG_SOURCE    = 404,      // Defines source of trigger.
    CV_CAP_PROP_XI_TRG_SOFTWARE  = 405,      // Generates an internal trigger. PRM_TRG_SOURCE must be set to TRG_SOFTWARE.
    CV_CAP_PROP_XI_GPI_SELECTOR  = 406,      // Selects general purpose input
    CV_CAP_PROP_XI_GPI_MODE      = 407,      // Set general purpose input mode
    CV_CAP_PROP_XI_GPI_LEVEL     = 408,      // Get general purpose level
    CV_CAP_PROP_XI_GPO_SELECTOR  = 409,      // Selects general purpose output
    CV_CAP_PROP_XI_GPO_MODE      = 410,      // Set general purpose output mode
    CV_CAP_PROP_XI_LED_SELECTOR  = 411,      // Selects camera signalling LED
    CV_CAP_PROP_XI_LED_MODE      = 412,      // Define camera signalling LED functionality
    CV_CAP_PROP_XI_MANUAL_WB     = 413,      // Calculates White Balance(must be called during acquisition)
    CV_CAP_PROP_XI_AUTO_WB       = 414,      // Automatic white balance
    CV_CAP_PROP_XI_AEAG          = 415,      // Automatic exposure/gain
    CV_CAP_PROP_XI_EXP_PRIORITY  = 416,      // Exposure priority (0.5 - exposure 50%, gain 50%).
    CV_CAP_PROP_XI_AE_MAX_LIMIT  = 417,      // Maximum limit of exposure in AEAG procedure
    CV_CAP_PROP_XI_AG_MAX_LIMIT  = 418,      // Maximum limit of gain in AEAG procedure
    CV_CAP_PROP_XI_AEAG_LEVEL    = 419,       // Average intensity of output signal AEAG should achieve(in %)
    CV_CAP_PROP_XI_TIMEOUT       = 420,       // Image capture timeout in milliseconds

    // Properties for Android cameras
    CV_CAP_PROP_ANDROID_FLASH_MODE = 8001,
    CV_CAP_PROP_ANDROID_FOCUS_MODE = 8002,
    CV_CAP_PROP_ANDROID_WHITE_BALANCE = 8003,
    CV_CAP_PROP_ANDROID_ANTIBANDING = 8004,
    CV_CAP_PROP_ANDROID_FOCAL_LENGTH = 8005,
    CV_CAP_PROP_ANDROID_FOCUS_DISTANCE_NEAR = 8006,
    CV_CAP_PROP_ANDROID_FOCUS_DISTANCE_OPTIMAL = 8007,
    CV_CAP_PROP_ANDROID_FOCUS_DISTANCE_FAR = 8008,

    // Properties of cameras available through AVFOUNDATION interface
    CV_CAP_PROP_IOS_DEVICE_FOCUS = 9001,
    CV_CAP_PROP_IOS_DEVICE_EXPOSURE = 9002,
    CV_CAP_PROP_IOS_DEVICE_FLASH = 9003,
    CV_CAP_PROP_IOS_DEVICE_WHITEBALANCE = 9004,
    CV_CAP_PROP_IOS_DEVICE_TORCH = 9005

    // Properties of cameras available through Smartek Giganetix Ethernet Vision interface
    /* --- Vladimir Litvinenko (litvinenko.vladimir@gmail.com) --- */
    ,CV_CAP_PROP_GIGA_FRAME_OFFSET_X = 10001,
    CV_CAP_PROP_GIGA_FRAME_OFFSET_Y = 10002,
    CV_CAP_PROP_GIGA_FRAME_WIDTH_MAX = 10003,
    CV_CAP_PROP_GIGA_FRAME_HEIGH_MAX = 10004,
    CV_CAP_PROP_GIGA_FRAME_SENS_WIDTH = 10005,
    CV_CAP_PROP_GIGA_FRAME_SENS_HEIGH = 10006
};

typedef void (FrameCopyFunction)(uint8_t *src, uint8_t *dst, uint32_t data_bytes, uint32_t width, uint32_t height, uint32_t bytes_per_pixel);

class videoDevice;
class videoInput{

public:
    videoInput();
    ~videoInput();

    //turns off console messages - default is to print messages
    static void setVerbose(bool _verbose);

    //Functions in rough order they should be used.
    static int listDevices(bool silent = false,bool need_get_support_size = false);

    static void GetSupportSize(IMoniker* pMoniker,const std::string& device_name,SupportSizeVector& support_sizes);

    //needs to be called after listDevices - otherwise returns NULL
    static char * getDeviceName(int deviceID);

    //choose to use callback based capture - or single threaded
    void setUseCallback(bool useCallback);

    //call before setupDevice
    //directshow will try and get the closest possible framerate to what is requested
    void setIdealFramerate(int deviceID, int idealFramerate);

    //some devices will stop delivering frames after a while - this method gives you the option to try and reconnect
    //to a device if videoInput detects that a device has stopped delivering frames.
    //you MUST CALL isFrameNew every app loop for this to have any effect
    void setAutoReconnectOnFreeze(int deviceNumber, bool doReconnect, int numMissedFramesBeforeReconnect);

    //Choose one of these five to setup your device
    bool setupDevice(int deviceID);
    bool setupDevice(int deviceID, int w, int h);
    bool setupDeviceFourcc(int deviceID, int w, int h,int fourcc);

    //These two are only for capture cards
    //USB and Firewire cameras souldn't specify connection
    bool setupDevice(int deviceID, int connection);
    bool setupDevice(int deviceID, int w, int h, int connection);

    bool setFourcc(int deviceNumber, int fourcc);

    //If you need to you can set your NTSC/PAL/SECAM
    //preference here. if it is available it will be used.
    //see #defines above for available formats - eg VI_NTSC_M or VI_PAL_B
    //should be called after setupDevice
    //can be called multiple times
    bool setFormat(int deviceNumber, int format);

    //Tells you when a new frame has arrived - you should call this if you have specified setAutoReconnectOnFreeze to true
    bool isFrameNew(int deviceID);

    bool isDeviceSetup(int deviceID);

    //Returns the pixels - flipRedAndBlue toggles RGB/BGR flipping - and you can flip the image too
    unsigned char * getPixels(int deviceID, bool flipRedAndBlue = true, bool flipImage = false);

    //Or pass in a buffer for getPixels to fill returns true if successful.
    bool getPixels(int id, unsigned char * pixels, bool flipRedAndBlue = true, bool flipImage = false);

    //Launches a pop up settings window
    //For some reason in GLUT you have to call it twice each time.
    void showSettingsWindow(int deviceID);

    //Manual control over settings thanks.....
    //These are experimental for now.
    bool setVideoSettingFilter(int deviceID, long Property, long lValue, long Flags = 0, bool useDefaultValue = false);
    bool setVideoSettingFilterPct(int deviceID, long Property, float pctValue, long Flags = 0);
    bool getVideoSettingFilter(int deviceID, long Property, long &min, long &max, long &SteppingDelta, long &currentValue, long &flags, long &defaultValue);

    bool setVideoSettingCamera(int deviceID, long Property, long lValue, long Flags = 0, bool useDefaultValue = false);
    bool setVideoSettingCameraPct(int deviceID, long Property, float pctValue, long Flags = 0);
    bool getVideoSettingCamera(int deviceID, long Property, long &min, long &max, long &SteppingDelta, long &currentValue, long &flags, long &defaultValue);

    //bool setVideoSettingCam(int deviceID, long Property, long lValue, long Flags = NULL, bool useDefaultValue = false);

    //get width, height and number of pixels
    int  getWidth(int deviceID);
    int  getHeight(int deviceID);
    int  getSize(int deviceID);
    int  getFourcc(int deviceID);
    double getFPS(int deviceID);

    //completely stops and frees a device
    void stopDevice(int deviceID);

    //as above but then sets it up with same settings
    bool restartDevice(int deviceID);

    //number of devices available
    int  devicesFound;

    // mapping from OpenCV CV_CAP_PROP to videoinput/dshow properties
    int getVideoPropertyFromCV(int cv_property);
    int getCameraPropertyFromCV(int cv_property);

    static char deviceNames[VI_MAX_CAMERAS][255];

    void SetFrameCopyFunction(FrameCopyFunction* frame_copy_fuc) {
        frame_copy = frame_copy_fuc;
    }

    void GetDeviceSupportSizes(const std::string& device_name, SupportSizeVector& support_sizes) {
        if(cameras_support_sizes_.find(device_name)!=cameras_support_sizes_.end()) {
            support_sizes = cameras_support_sizes_[device_name];
        }
    }
private:
    void setPhyCon(int deviceID, int conn);
    void setAttemptCaptureSize(int deviceID, int w, int h, GUID mediaType);
    bool setup(int deviceID);
    void processPixels(unsigned char * src, unsigned char * dst, int width, int height, bool bRGB, bool bFlip);
    int  start(int deviceID, videoDevice * VD);
    int  getDeviceCount();
    void getMediaSubtypeAsString(GUID type, char * typeAsString);
    GUID *getMediaSubtypeFromFourcc(int fourcc);
    int    getFourccFromMediaSubtype(GUID type);

    void getVideoPropertyAsString(int prop, char * propertyAsString);
    void getCameraPropertyAsString(int prop, char * propertyAsString);

    HRESULT getDevice(IBaseFilter **pSrcFilter, int deviceID, WCHAR * wDeviceName, char * nDeviceName);
    static HRESULT ShowFilterPropertyPages(IBaseFilter *pFilter);
    static HRESULT ShowStreamPropertyPages(IAMStreamConfig  *pStream);

    HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath);
    HRESULT routeCrossbar(ICaptureGraphBuilder2 **ppBuild, IBaseFilter **pVidInFilter, int conType, GUID captureMode);

    //don't touch
    static bool comInit();
    static bool comUnInit();

    int  connection;
    int  callbackSetCount;
    bool bCallback;

    GUID CAPTURE_MODE;

    //Extra video subtypes
    GUID MEDIASUBTYPE_Y800;
    GUID MEDIASUBTYPE_Y8;
    GUID MEDIASUBTYPE_GREY;

    videoDevice * VDList[VI_MAX_CAMERAS];
    GUID mediaSubtypes[VI_NUM_TYPES];
    long formatTypes[VI_NUM_FORMATS];

    static void __cdecl basicThread(void * objPtr);

    FrameCopyFunction*  frame_copy{ nullptr };

    static std::map<std::string,SupportSizeVector> cameras_support_sizes_; 
};

}; /// end of namespace xreal_dshow