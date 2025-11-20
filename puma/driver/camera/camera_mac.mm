/*M///////////////////////////////////////////////////////////////////////////////////////
//
// IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
// By downloading, copying, installing or using the software you agree to this license.
// If you do not agree to this license, do not download, install,
// copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// * Redistribution's of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// * Redistribution's in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// * The name of the copyright holders may not be used to endorse or promote products
// derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the contributor be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*////////////////////////////////////////////////////////////////////////////////////////

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <stdio.h>
#include <Availability.h>
#import <AVFoundation/AVFoundation.h>
#include <puma/driver/camera/camera_mac.h>
#include <puma/util/log.h>
#include <libyuv.h>

using namespace libyuv;
/********************** Declaration of class headers ************************/

/*****************************************************************************
 *
 * CaptureDelegate Declaration.
 *
 * CaptureDelegate is notified on a separate thread by the OS whenever there
 *   is a new frame. When "updateImage" is called from the main thread, it
 *   copies this new frame into an IplImage, but only if this frame has not
 *   been copied before. When "getOutput" is called from the main thread,
 *   it gives the last copied IplImage.
 *
 *****************************************************************************/


@interface NRCaptureDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
{
    NSCondition *mHasNewFrame;
    CVPixelBufferRef mGrabbedPixels;
    CVImageBufferRef mCurrentImageBuffer;
    uint8_t  *mOutImagedata;
    CopyFrameFunction *mCopyFrameFunction;
    //frame_t mDeviceImage;
    frame_t mOutImage;
    size_t    currSize;
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection;

- (BOOL)grabImageUntilDate: (NSDate *)limit;
- (int)updateImage;
- (frame_t*)getOutput;
- (void)setCopyFrameFunction: (CopyFrameFunction *)cf;
@end

/*****************************************************************************
 *
 * CvCaptureCAM Declaration.
 *
 * CvCaptureCAM is the instantiation of a capture source for cameras.
 *
 *****************************************************************************/

enum CAMERA_PROP_TYPE {
    CV_CAP_PROP_FRAME_WIDTH    =0,
    CV_CAP_PROP_FRAME_HEIGHT   =1,
    CV_CAP_PROP_FPS            =2,
    CV_CAP_PROP_AUTO_EXPOSURE  =3,
    CV_CAP_PROP_EXPOSURE_VALUE =4,
    CV_CAP_PROP_GAIN_VALUE     =5,
};

class CvCaptureCAM {
public:
    CvCaptureCAM();

    ~CvCaptureCAM();
    void setProductNames(const std::vector<std::string>& productNames);
    bool grabFrame(double timeOut);
    frame_t* retrieveFrame();
    double getProperty(int property_id) const;
    bool setProperty(int property_id, double value);
    int startCaptureDevice();
    void stopCaptureDevice();
    void setWidthHeight();
    void setCopyFrameFunction( CopyFrameFunction* copy_frame);

    int getMatchedProperty(int property_id) const;

    void setNeedMatchMinHeight(bool match_min_height) { need_match_min_height = match_min_height;}
private:
    AVCaptureSession            *mCaptureSession;
    AVCaptureDeviceInput        *mCaptureDeviceInput;
    AVCaptureVideoDataOutput    *mCaptureVideoDataOutput;
    AVCaptureDevice             *mCaptureDevice;
    NRCaptureDelegate             *mCapture;

    int width;
    int height;
    int fps;
    int settingWidth;
    int settingHeight;
    int started;
    std::vector<std::string> product_names;
    CopyFrameFunction*  copy_frame_func;
    bool need_match_min_height{false};
};


/********************** Implementation of Classes ****************************/

/*****************************************************************************
 *
 * CvCaptureCAM Implementation.
 *
 * CvCaptureCAM is the instantiation of a capture source for cameras.
 *
 *****************************************************************************/

CvCaptureCAM::CvCaptureCAM() {
    mCaptureSession = nil;
    mCaptureDeviceInput = nil;
    mCaptureVideoDataOutput = nil;
    mCaptureDevice = nil;
    mCapture = nil;
    copy_frame_func = nil;

    width = 0;
    height = 0;
    settingWidth = 0;
    settingHeight = 0;

    started = 0;
}

CvCaptureCAM::~CvCaptureCAM() {
    if (started) {
        stopCaptureDevice();
    }    
}

void CvCaptureCAM::setProductNames(const std::vector<std::string>& productNames) {
    product_names = productNames;
}

void CvCaptureCAM::setCopyFrameFunction( CopyFrameFunction* copy_frame){
    copy_frame_func = copy_frame;
}

bool CvCaptureCAM::grabFrame(double timeOut) {
    NSAutoreleasePool *localpool = [[NSAutoreleasePool alloc] init];

    bool isGrabbed = false;
    NSDate *limit = [NSDate dateWithTimeIntervalSinceNow: timeOut];
    if ( [mCapture grabImageUntilDate: limit] ) {
        [mCapture updateImage];
        isGrabbed = true;
    }

    [localpool drain];
    return isGrabbed;
}

frame_t* CvCaptureCAM::retrieveFrame() {
    return [mCapture getOutput];
}

void CvCaptureCAM::stopCaptureDevice() {
    NSAutoreleasePool *localpool = [[NSAutoreleasePool alloc] init];

    [mCaptureSession stopRunning];

    [mCaptureSession release];
    [mCaptureDeviceInput release];
    // [mCaptureDevice release]; fix #7833

    [mCaptureVideoDataOutput release];
    [mCapture release];

    [localpool drain];

    started = 0;
}

int CvCaptureCAM::startCaptureDevice() {
    NSAutoreleasePool *localpool = [[NSAutoreleasePool alloc] init];

#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 101400
    if (@available(macOS 10.14, *))
    {
        AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
        if (status == AVAuthorizationStatusDenied)
        {
            PUMA_LOG_ERROR("Camera access has been denied. Either run 'tccutil reset Camera' "
                            "command in same terminal to reset application authorization status, "
                            "either modify 'System Preferences -> Security & Privacy -> Camera' "
                            "settings for your application.");
            [localpool drain];
            return 0;
        }
        else if (status != AVAuthorizationStatusAuthorized)
        {
            PUMA_LOG_ERROR("Not authorized to capture video (status {}), requesting...", status);
            [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL) { /* we don't care */}];
            if ([NSThread isMainThread])
            {
                // we run the main loop for 0.1 sec to show the message
                [[NSRunLoop mainRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:0.1]];
            }
            else
            {
                PUMA_LOG_ERROR("Can not spin main run loop from other thread, set "
                                "OPENCV_AVFOUNDATION_SKIP_AUTH=1 to disable authorization request "
                                "and perform it in your application.");
            }
            [localpool drain];
            return 0;
        } else {
            PUMA_LOG_INFO("CameraMac authorization status {}", status);
        }
    }
#endif

    // get capture device
    NSArray *devices = [[AVCaptureDevice devicesWithMediaType: AVMediaTypeVideo]
            arrayByAddingObjectsFromArray:[AVCaptureDevice devicesWithMediaType:AVMediaTypeMuxed]];
            
    if ( devices.count == 0 ) {
        PUMA_LOG_ERROR("AVFoundation didn't find any attached Video Input Devices!");
        [localpool drain];
        return 0;
    }

    int cameraNum = 0;
    for (; NSUInteger(cameraNum) < devices.count; cameraNum++) {
        mCaptureDevice = devices[cameraNum];
        std::string localizedName = std::string([mCaptureDevice.localizedName UTF8String]);
        //bool camera_found = false;
        //for (auto name : product_names) {
        //    if (localizedName.find(name) != std::string::npos) {
        //        PUMA_LOG_INFO("Found product : {}", name);
        //        camera_found = true;
        //        break;
        //    } else {
        //        PUMA_LOG_INFO("Found product : {} expect product: {}", localizedName, name);
        //    }
        //}
        if (localizedName.find("Camera-OV580") != std::string::npos)
        {
            PUMA_LOG_INFO("Found product : {}", "Camera-OV580");
            break;
        }
        bool camera_found = false;
        for (auto name : product_names) {
            if (localizedName.find(name) != std::string::npos)
            {
                PUMA_LOG_INFO("Found product : {}", name);
                camera_found = true;
                break;
            } else {
                PUMA_LOG_INFO("Found product : {} expect product: {}", localizedName, name);
            }
        }
        if (camera_found) break;
    }

    if (devices.count <= NSUInteger(cameraNum) ) {
        PUMA_LOG_ERROR("Out device of bound (0-{}): {}", devices.count-1, cameraNum);
        [localpool drain];
        return 0;
    }

    mCaptureDevice = devices[cameraNum];

    if ( ! mCaptureDevice ) {
        PUMA_LOG_ERROR("Device {} not able to use.", cameraNum);
        [localpool drain];
        return 0;
    }

    // get input device
    NSError *error = nil;
    mCaptureDeviceInput = [[AVCaptureDeviceInput alloc] initWithDevice: mCaptureDevice
                                                                 error: &error];
    if ( error ) {
        PUMA_LOG_ERROR("Error in [AVCaptureDeviceInput initWithDevice:error:]");
        NSLog(@"OpenCV: %@", error.localizedDescription);
        [localpool drain];
        return 0;
    }

    // create output
    mCapture = [[NRCaptureDelegate alloc] init];
    [mCapture setCopyFrameFunction: copy_frame_func];
    mCaptureVideoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
    dispatch_queue_t queue = dispatch_queue_create("cameraQueue", DISPATCH_QUEUE_SERIAL);
    [mCaptureVideoDataOutput setSampleBufferDelegate: mCapture queue: queue];
    dispatch_release(queue);

    // OSType pixelFormat = kCVPixelFormatType_32BGRA;
    OSType pixelFormat = kCVPixelFormatType_422YpCbCr8_yuvs;
    NSDictionary *pixelBufferOptions;

    if (width > 0 && height > 0) {
        pixelBufferOptions =
            @{
                (id)kCVPixelBufferWidthKey:  @(1.0*width),
                (id)kCVPixelBufferHeightKey: @(1.0*height),
                (id)kCVPixelBufferPixelFormatTypeKey: @(pixelFormat)
            };
    } else {
        pixelBufferOptions =
            @{
                (id)kCVPixelBufferPixelFormatTypeKey: @(pixelFormat)
            };
    }
    mCaptureVideoDataOutput.videoSettings = pixelBufferOptions;
    mCaptureVideoDataOutput.alwaysDiscardsLateVideoFrames = YES;

    // create session
    mCaptureSession = [[AVCaptureSession alloc] init];
    mCaptureSession.sessionPreset = AVCaptureSessionPresetMedium;
    [mCaptureSession addInput: mCaptureDeviceInput];
    [mCaptureSession addOutput: mCaptureVideoDataOutput];

    [mCaptureSession startRunning];

    // flush old position image
    grabFrame(1);

    [localpool drain];

    started = 1;

    return 1;
}

void CvCaptureCAM::setWidthHeight() {
    NSMutableDictionary *pixelBufferOptions = [mCaptureVideoDataOutput.videoSettings mutableCopy];

    int matched_width = width;
    int matched_height = height;
    if(need_match_min_height) {
        if (mCaptureDevice.formats.count > 0) {
            AVCaptureDeviceFormat *firstFormat = [mCaptureDevice.formats firstObject];
            CMVideoDimensions dimensions = CMVideoFormatDescriptionGetDimensions(firstFormat.formatDescription);
            matched_height = dimensions.height;
            matched_width  = dimensions.width;
        }
        for (AVCaptureDeviceFormat *format in mCaptureDevice.formats) {
            CMVideoDimensions dimensions = CMVideoFormatDescriptionGetDimensions(format.formatDescription);
            PUMA_LOG_TRACE("Supported format - width: {}, height: {}", dimensions.width, dimensions.height);
            if(dimensions.height < matched_width) {
                matched_height = dimensions.height;
                matched_width  = dimensions.width;
            }
        }
        width = matched_width;
        height = matched_height;
        PUMA_LOG_TRACE("matched size - width: {}, height: {}", matched_width, matched_height);
    }

    while ( true ) {
        // auto matching
        pixelBufferOptions[(id)kCVPixelBufferWidthKey]  = @(1.0*width);
        pixelBufferOptions[(id)kCVPixelBufferHeightKey] = @(1.0*height);
        mCaptureVideoDataOutput.videoSettings = pixelBufferOptions;

        if(need_match_min_height) {
            break;
        }
        else{
            // compare matched size and my options
            CMFormatDescriptionRef format = mCaptureDevice.activeFormat.formatDescription;
            CMVideoDimensions deviceSize = CMVideoFormatDescriptionGetDimensions(format);
            PUMA_LOG_TRACE("CvCaptureCAM::setWidthHeight device:width={},height={}, setting width={},height={}",deviceSize.width,deviceSize.height,width,height);
            if ( deviceSize.width == width && deviceSize.height == height ) {
                break;
            }

            // fit my options to matched size
            width = deviceSize.width;
            height = deviceSize.height;
        }
    }

    // flush old size image
    grabFrame(1);

    [pixelBufferOptions release];
}

int CvCaptureCAM::getMatchedProperty(int property_id) const {
    int retval = 0;
    switch (property_id) {
        case CV_CAP_PROP_FRAME_WIDTH:
            retval = width;
            break;
        case CV_CAP_PROP_FRAME_HEIGHT:
            retval = height;
            break;
        case CV_CAP_PROP_FPS:
            retval = fps;
            break;
        default:
            break;
    }
    return retval;
}


double CvCaptureCAM::getProperty(int property_id) const{
    NSAutoreleasePool *localpool = [[NSAutoreleasePool alloc] init];

    CMFormatDescriptionRef format = mCaptureDevice.activeFormat.formatDescription;
    CMVideoDimensions s1 = CMVideoFormatDescriptionGetDimensions(format);
    double retval = 0;

    switch (property_id) {
        case CV_CAP_PROP_FRAME_WIDTH:
            retval = s1.width;
            break;
        case CV_CAP_PROP_FRAME_HEIGHT:
            retval = s1.height;
            break;
        case CV_CAP_PROP_FPS:
            {
                CMTime frameDuration = mCaptureDevice.activeVideoMaxFrameDuration;
                retval = frameDuration.timescale / double(frameDuration.value);
            }
            break;
        default:
            break;
    }

    [localpool drain];
    return retval;
}

bool CvCaptureCAM::setProperty(int property_id, double value) {
    NSAutoreleasePool *localpool = [[NSAutoreleasePool alloc] init];

    bool isSucceeded = false;
    int property_value = 0;

    switch (property_id) {
        case CV_CAP_PROP_FRAME_WIDTH:
            width = value;
            settingWidth = 1;
            if (settingWidth && settingHeight) {
                setWidthHeight();
                settingWidth = 0;
                settingHeight = 0;
            }
            isSucceeded = true;
            break;
        case CV_CAP_PROP_FRAME_HEIGHT:
            height = value;
            settingHeight = 1;
            if (settingWidth && settingHeight) {
                setWidthHeight();
                settingWidth = 0;
                settingHeight = 0;
            }
            isSucceeded = true;
            break;
        case CV_CAP_PROP_FPS:
            if(need_match_min_height && (height == kFloraImageMinHeight))  {  //only for flora when camera image is separated transmission.
                value = 60;
                fps = 60;
            }
            if ( [mCaptureDevice lockForConfiguration: NULL] ) {
                NSArray * ranges = mCaptureDevice.activeFormat.videoSupportedFrameRateRanges;
                AVFrameRateRange *matchedRange = ranges[0];
                double minDiff = fabs(matchedRange.maxFrameRate - value);
                int i=0;
                for ( AVFrameRateRange *range in ranges ) {
                    PUMA_LOG_TRACE("{},maxFrameRate:{},minFrameRate:{},,input frame rate:{}",i,range.maxFrameRate,range.minFrameRate,value);
                    i++;
                    double diff = fabs(range.maxFrameRate - value);
                    if ( diff < minDiff ) {
                        minDiff = diff;
                        matchedRange = range;
                    }
                }
                mCaptureDevice.activeVideoMinFrameDuration = matchedRange.minFrameDuration;
                mCaptureDevice.activeVideoMaxFrameDuration = matchedRange.minFrameDuration;
                isSucceeded = true;
                [mCaptureDevice unlockForConfiguration];
            }
            break;
        case CV_CAP_PROP_AUTO_EXPOSURE:
            property_value = value;
            if ( [mCaptureDevice lockForConfiguration: NULL] ) {
                if ( property_value == 1 ) {
                    if ( [mCaptureDevice isExposureModeSupported : AVCaptureExposureModeContinuousAutoExposure] ) {
                        PUMA_LOG_INFO("AVCaptureExposureModeContinuousAutoExposure");
                        mCaptureDevice.exposureMode = AVCaptureExposureModeContinuousAutoExposure;
                    }
                } else {
                    PUMA_LOG_WARN("MacOS not supported custom exposure, i.e. manual exposure");
                }    
                
                isSucceeded = true;
                [mCaptureDevice unlockForConfiguration];
            }
            break;
        default:
            break;
    }

    [localpool drain];
    return isSucceeded;
}

/*****************************************************************************
 *
 * CaptureDelegate Implementation.
 *
 * CaptureDelegate is notified on a separate thread by the OS whenever there
 *   is a new frame. When "updateImage" is called from the main thread, it
 *   copies this new frame into an IplImage, but only if this frame has not
 *   been copied before. When "getOutput" is called from the main thread,
 *   it gives the last copied IplImage.
 *
 *****************************************************************************/


@implementation NRCaptureDelegate

- (id)init {
    [super init];
    mHasNewFrame = [[NSCondition alloc] init];
    mCurrentImageBuffer = NULL;
    mGrabbedPixels = NULL;
    mOutImagedata = NULL;
    mCopyFrameFunction = NULL;
    //mDeviceImage.data = NULL;
    mOutImage.data = NULL;
    currSize = 0;
    return self;
}

-(void)dealloc {
    free(mOutImagedata);
    CVBufferRelease(mCurrentImageBuffer);
    CVBufferRelease(mGrabbedPixels);
    [mHasNewFrame release];
    [super dealloc];
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection {
    (void)captureOutput;
    (void)sampleBuffer;
    (void)connection;

    //PUMA_LOG_INFO("didOutputSampleBuffer");
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    CVBufferRetain(imageBuffer);

    [mHasNewFrame lock];

    CVBufferRelease(mCurrentImageBuffer);
    mCurrentImageBuffer = imageBuffer;
    [mHasNewFrame signal];

    [mHasNewFrame unlock];

}

-(frame_t*) getOutput {
    return &mOutImage;
}

-(void) setCopyFrameFunction: (CopyFrameFunction *)cf {
    mCopyFrameFunction = cf;
}

-(BOOL) grabImageUntilDate: (NSDate *)limit {
    BOOL isGrabbed = NO;
    [mHasNewFrame lock];

    if ( mGrabbedPixels ) {
        CVBufferRelease(mGrabbedPixels);
    }
    if ( [mHasNewFrame waitUntilDate: limit] ) {
        isGrabbed = YES;
        mGrabbedPixels = CVBufferRetain(mCurrentImageBuffer);
    }

    [mHasNewFrame unlock];
    return isGrabbed;
}

#pragma pack(1)
typedef struct AirProMetaData {
    uint64_t timestamp;     //  ns
    uint16_t type;          //  default 0
    uint64_t sync_ts;       //  default 0
    uint32_t frame_number;  //
    uint32_t exposure;      // ns
    uint32_t gain;          //
    uint32_t signature;     //  default 0
    uint8_t cam_count;      // max 4
    uint32_t cam0_offset;   // cam0 start offset in memory
    uint32_t cam1_offset;   // cam1 start offset in memory
    uint32_t cam2_offset;   // cam2 start offset in memory
    uint32_t cam3_offset;   // cam3 start offset in memory
    uint16_t width;         // camera image width
    uint16_t height;        // camera image height
    uint32_t stride;        // stride of line
} AirProMetaData;

#pragma pack()

-(int) updateImage {
    if ( ! mGrabbedPixels ) {
        PUMA_LOG_INFO("mGrabbedPixels {}", (void*)mGrabbedPixels);
        return 0;
    }

    CVPixelBufferLockBaseAddress(mGrabbedPixels, 0);
    void *baseaddress = CVPixelBufferGetBaseAddress(mGrabbedPixels);

    size_t width = CVPixelBufferGetWidth(mGrabbedPixels);
    size_t height = CVPixelBufferGetHeight(mGrabbedPixels);
    size_t rowBytes = CVPixelBufferGetBytesPerRow(mGrabbedPixels);
    OSType pixelFormat = CVPixelBufferGetPixelFormatType(mGrabbedPixels);

    if ( rowBytes == 0 ) {
        PUMA_LOG_ERROR("error: rowBytes == 0");
        CVPixelBufferUnlockBaseAddress(mGrabbedPixels, 0);
        CVBufferRelease(mGrabbedPixels);
        mGrabbedPixels = NULL;
        return 0;
    }

    if ( currSize != rowBytes*height ) {
        currSize = rowBytes*height;
        free(mOutImagedata);
        mOutImagedata = reinterpret_cast<uint8_t*>(malloc(currSize));
    }

    mOutImage.data_bytes = currSize;
    mOutImage.width = width;
    mOutImage.height = height;
    mOutImage.step = rowBytes;
    mOutImage.bytes_per_pixel = rowBytes/width;
    mOutImage.data = mOutImagedata;
    PUMA_LOG_TRACE("mOutImage data_bytes:{} width:{} height:{} step:{} bytes_per_pixel:{}", currSize, width, height, rowBytes, rowBytes/width)

    if ( pixelFormat == kCVPixelFormatType_422YpCbCr8_yuvs ) {
        PUMA_LOG_TRACE("rowBytes= {}", rowBytes)
        if(mCopyFrameFunction!=nil) {
            mCopyFrameFunction(reinterpret_cast<uint8_t*>(baseaddress),mOutImagedata,mOutImage.data_bytes,mOutImage.width,mOutImage.height,mOutImage.bytes_per_pixel);
        }
        else {
            CopyPlane(reinterpret_cast<uint8_t*>(baseaddress), mOutImage.data_bytes, mOutImagedata, mOutImage.data_bytes, mOutImage.data_bytes, 1);
        }
    } else {
        PUMA_LOG_ERROR("unknown pixel format 0x{:8x}", pixelFormat);
        CVPixelBufferUnlockBaseAddress(mGrabbedPixels, 0);
        CVBufferRelease(mGrabbedPixels);
        mGrabbedPixels = NULL;
        return 0;
    }

    CVPixelBufferUnlockBaseAddress(mGrabbedPixels, 0);
    CVBufferRelease(mGrabbedPixels);
    mGrabbedPixels = NULL;

    return 1;
}

@end

//---------------------------NR Code------------------------------//
CameraMac::CameraMac(std::function<void (const GenericShmWrapper&)> _callback) : CameraDriver(_callback) {
    mac_cam_.reset(new CvCaptureCAM());
}

CameraMac::~CameraMac() {
    stop();
}

bool CameraMac::start() {
    for (auto name : product_name_) {
        PUMA_LOG_INFO("finding from product name list : {}", name);
    }
    if (product_name_.empty()) {
        PUMA_LOG_WARN("no camera name read from mcu proto");
    }
    mac_cam_->setProductNames(product_name_);
    mac_cam_->setCopyFrameFunction(copy_frame_);
    if (!mac_cam_->startCaptureDevice()) {
        return false;
    }
    if(device_type_ == DEVICE_TYPE_FLORA) {
        mac_cam_->setNeedMatchMinHeight(true);
    }
    mac_cam_->setProperty(CV_CAP_PROP_FRAME_WIDTH, info_.width);
    mac_cam_->setProperty(CV_CAP_PROP_FRAME_HEIGHT, info_.height);
    mac_cam_->setProperty(CV_CAP_PROP_FPS, info_.frame_rate);

    if(device_type_ == DEVICE_TYPE_FLORA) {
        info_.width = mac_cam_->getMatchedProperty(CV_CAP_PROP_FRAME_WIDTH);
        info_.height = mac_cam_->getMatchedProperty(CV_CAP_PROP_FRAME_WIDTH);
        info_.frame_rate = mac_cam_->getMatchedProperty(CV_CAP_PROP_FRAME_WIDTH);
        PUMA_LOG_TRACE("get matched property, width:{},height:{},frame_rate:{}",info_.width, info_.height, info_.frame_rate);
    }

    setStartTime();
    return CameraDriver::start();
}
    
void CameraMac::stop() {
    if (running_ == true) {
        CameraDriver::stop();
    }
    if (mac_cam_) {
        mac_cam_->stopCaptureDevice();
        mac_cam_.reset();
    }
}

void CameraMac::setAutoExposure(int exposure_mode) {
    mac_cam_->setProperty(CV_CAP_PROP_AUTO_EXPOSURE, exposure_mode);
}

void CameraMac::setExposureTime(double ) {
    // mac_cam_->setProperty(_exposure_time);
}

void CameraMac::setGain(int ) {
    // mac_cam_->setProperty(_gain);
}

bool CameraMac::setVideoProperties(uint32_t prop, uint8_t ) {
    PUMA_LOG_WARN("Set VideoProperty {} is not supported on MacOS yet", prop);
    return false;
}

bool CameraMac::getVideoProperties(uint32_t prop, uint8_t& ) {
    PUMA_LOG_WARN("Get VideoProperty {} is not supported on MacOS yet", prop);
    return false;
}

void CameraMac::run() {    
    frame_t * ptr = (frame_t *)curr_frame_.reserved;
    ptr->device_start_time = startTime();
    curr_frame_.reserved_size = sizeof(frame_t);
    curr_frame_.type = 1;

    while (running_) {
        if (!mac_cam_->grabFrame(2)) {
            PUMA_LOG_ERROR("grabFrame no images")
            break;
        }
        frame_t * frame = mac_cam_->retrieveFrame();
        ptr->data               = frame->data;
        ptr->step               = frame->step;
        ptr->data_bytes         = frame->data_bytes;
        ptr->height             = frame->height;
        ptr->width              = frame->width;
        ptr->bytes_per_pixel    = frame->bytes_per_pixel; //### xreal use this as camera count
        ptr->sof        = 0;
        if (frame_callback_)
            frame_callback_(curr_frame_);
    }
    PUMA_LOG_TRACE("CameraMac run quit");
}

#pragma clang diagnostic pop
