#include "camera_dshow.h"
#if defined(PUMA_SYSTEM_WINDOWS)
#include <stdlib.h>
#include <unistd.h>
#include <puma/util/log.h>
#include <puma/common/common.h>

CameraDshow::CameraDshow(std::function<void (const GenericShmWrapper&)> _callback) :
    CameraDriver(_callback) {

    /// only support 20191212 and later OV FW
    {
        max_gain_ = 255;
        min_gain_ = 1;
        max_valid_gain_ = max_gain_*3.0/8.0;
        min_valid_gain_ = 16;
        max_valid_exposure_time_ = -5;
        min_valid_exposure_time_ = -13;
    }
}


CameraDshow::~CameraDshow() {
    stop();
    delete [] video_input_buffer_;
    video_input_buffer_ = nullptr;
}

bool CameraDshow::start() {
    return CameraDriver::start();
}

void CameraDshow::stop() {
    if (true == running_) {
        CameraDriver::stop();
        if (video_input_device_ >= 0 && video_input_.isDeviceSetup(video_input_device_)) {
            video_input_.stopDevice(video_input_device_);
        }
    }
}

bool CameraDshow::find_device() {
    bool is_flora_device = (device_type_ == DEVICE_TYPE_FLORA);
    int devices = video_input_.listDevices(false,is_flora_device);
    if (devices == 0) {
        PUMA_LOG_ERROR("There no available camera");
        return false;
    }

    if (product_name_.empty()) {
        PUMA_LOG_ERROR("No camera specified");
        return false;
    }
    
    std::string device_name;
    for(int i = 0; i < devices; ++i) {
        device_name = video_input_.getDeviceName(i);
        for (auto& name : product_name_) {
            PUMA_LOG_TRACE("Specified camera = {}", name.c_str());
            if(device_name.find(name) != std::string::npos){
                if(is_flora_device) {
                    SupportSizeVector support_sizes;
                    video_input_.GetDeviceSupportSizes(device_name,support_sizes);
                    if(!support_sizes.empty()) {
                        CameraSupportSize min_height_size = support_sizes[0];
                        for(size_t arr_index = 1; arr_index< support_sizes.size();arr_index++){
                            CameraSupportSize temp_size = support_sizes[arr_index];
                            if(temp_size.height < min_height_size.height) {
                                min_height_size = temp_size;
                            }
                        }
                        PUMA_LOG_INFO("camera set height:{}, width:{}",min_height_size.height, min_height_size.width);
                        info_.width = min_height_size.width;
                        info_.height = min_height_size.height;
                        if(info_.width == kFloraImageMinHeight) {
                            info_.frame_rate = 60;
                        }
                    }
                    else {
                        PUMA_LOG_WARN("camera support_sizes is null");
                    }
                }
                video_input_device_ = i;
                PUMA_LOG_INFO("XREAL camera found.");
                video_input_buffer_ = new unsigned char[info_.width * info_.height * info_.channels];
                return true;
            }
        }
    }

    PUMA_LOG_FATAL("XREAL camera not found.");
    return false;
}

void CameraDshow::setAutoExposure(int _auto_exposure) {
    CameraDriver::setAutoExposure(_auto_exposure);
    setProperty(xreal_dshow::CV_CAP_PROP_AUTO_EXPOSURE, AE_mode_ == AEAGMode::AE_MODE_ISP);
}

void CameraDshow::setExposureTime(double _exposure_time) {
    CameraDriver::setExposureTime(_exposure_time);
    if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
        return;
    }

    /// Temporary exposure unit is unknown for windows,
    /// which range is [-13,-5] via OpenCV
    setProperty(xreal_dshow::CV_CAP_PROP_EXPOSURE, _exposure_time/* *10 */);
}

bool CameraDshow::setVideoProperties(uint32_t prop, uint8_t value) {   
    return false;
}

bool CameraDshow::getVideoProperties(uint32_t prop, uint8_t& value) {
    return false;
}

void CameraDshow::setGain(int _gain) {
    CameraDriver::setGain(_gain);
    if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
        return;
    }
    setProperty(xreal_dshow::CV_CAP_PROP_GAIN, _gain);
}

void CameraDshow::run() {
    CoInitialize(0);
    do {
        if (!find_device()) { break; }
        
        if (video_input_device_ < 0) {
            break;
        }
        PUMA_LOG_TRACE("video_input_device_ = {}, width = {}, height = {}, channels = {}",
            video_input_device_, info_.width, info_.height, info_.channels);
        running_ = video_input_.setupDevice(video_input_device_, 
                                            info_.width, 
                                            info_.height); 
        setStartTime();

        if (video_input_.isDeviceSetup(video_input_device_)) {
            PUMA_LOG_INFO("Camera open success");
        }

        if (!video_input_.isDeviceSetup(video_input_device_)) {
            running_ = false;
            if (video_input_.isDeviceSetup(video_input_device_)) {
                video_input_.stopDevice(video_input_device_);
            }
            PUMA_LOG_ERROR("Camera open failure");
            break;
        }

        video_input_.SetFrameCopyFunction(copy_frame_);
        int frame_count = 0;

        frame_t * ptr = (frame_t *)curr_frame_.reserved;
        ptr->device_start_time = startTime();
        curr_frame_.reserved_size = sizeof(frame_t);
        curr_frame_.type = 1;

        while (running_) {
            bool cap_ret = video_input_.getPixels(video_input_device_, video_input_buffer_, false, false);   //fills pixels as a BGR (for openCV) unsigned char array - no flipping
            if (!cap_ret) {
                restart_flag_ = 1;
                PUMA_LOG_ERROR("CAPTURE ERROR, need restart device ...");
                break;
            }

            restart_flag_ = 0;
            ptr->data = (uint8_t*)video_input_buffer_;
            ptr->step = info_.width * info_.channels;
            ptr->data_bytes = info_.height * ptr->step;
            ptr->height = info_.height;
            ptr->width = info_.width;
            ptr->bytes_per_pixel = info_.channels;
            ptr->sof = 0;
            if (frame_callback_)
                frame_callback_(curr_frame_);

            /// May not work in windows system
            //softAEAG(*ptr);

            if (frame_count < 5) {
                frame_count++;
                if (frame_count == 5) {
                    frame_count++;
                    /// set exposure
                    setAutoExposure(static_cast<int>(AE_mode_));
                    /// set exposure time
                    setExposureTime(exposure_time_);
                    /// set Gain
                    setGain(gain_);
                    
                    uint8_t value = 0;
                    getVideoProperties(VideoProperty::CAMERA_MAX_EXPOSURE_TIME, value);
                    PUMA_LOG_INFO("Camera max exposure time {}", value);
                    getVideoProperties(VideoProperty::CAMERA_LUMA_VALUE, value);
                    PUMA_LOG_INFO("Camera luma value {}", value);
                    bool success = false;
                    do {
                        success = setVideoProperties(VideoProperty::CAMERA_MAX_EXPOSURE_TIME, max_exposure_time_);
                        if (!success) { break; }
                        success = setVideoProperties(VideoProperty::CAMERA_LUMA_VALUE, luma_value_);
                    } while (0);
                    if (!success) {
                        PUMA_LOG_WARN("Failed to set properties, which may affect image quality");
                    }

                }
            }
        }

        if (0 == restart_flag_) {
            break;
        } else {
            if (!running_) break;
            restartDevice();
        }
    } while(1);
    CoUninitialize();
    PUMA_LOG_INFO("Windows camera thread exit.");
}

int CameraDshow::restartDevice() {
    if (video_input_.isDeviceSetup(video_input_device_)) {
        video_input_.stopDevice(video_input_device_);
    }
    restart_flag_ = 0;

   // PUMA_LOG_ERROR("Camera restart failure");
    return -1;
}


double CameraDshow::getProperty( int property_id ) {
    long min_value,max_value,stepping_delta,current_value,flags,defaultValue;

    // image format proprrties
    switch( property_id )
    {
    case xreal_dshow::CV_CAP_PROP_FRAME_WIDTH:
        return video_input_.getWidth(video_input_device_);

    case xreal_dshow::CV_CAP_PROP_FRAME_HEIGHT:
        return video_input_.getHeight(video_input_device_);

    case xreal_dshow::CV_CAP_PROP_FOURCC:
        return video_input_.getFourcc(video_input_device_);

    case xreal_dshow::CV_CAP_PROP_FPS:
        return video_input_.getFPS(video_input_device_);
    }

    // video filter properties
    switch( property_id )
    {
    case xreal_dshow::CV_CAP_PROP_BRIGHTNESS:
    case xreal_dshow::CV_CAP_PROP_CONTRAST:
    case xreal_dshow::CV_CAP_PROP_HUE:
    case xreal_dshow::CV_CAP_PROP_SATURATION:
    case xreal_dshow::CV_CAP_PROP_SHARPNESS:
    case xreal_dshow::CV_CAP_PROP_GAMMA:
    case xreal_dshow::CV_CAP_PROP_MONOCROME:
    case xreal_dshow::CV_CAP_PROP_WHITE_BALANCE_BLUE_U:
    case xreal_dshow::CV_CAP_PROP_BACKLIGHT:
    case xreal_dshow::CV_CAP_PROP_GAIN:
        if (video_input_.getVideoSettingFilter(video_input_device_,video_input_.getVideoPropertyFromCV(property_id),min_value,max_value,stepping_delta,current_value,flags,defaultValue) ) return (double)current_value;
    }

    // camera properties
    switch( property_id )
    {
    case xreal_dshow::CV_CAP_PROP_PAN:
    case xreal_dshow::CV_CAP_PROP_TILT:
    case xreal_dshow::CV_CAP_PROP_ROLL:
    case xreal_dshow::CV_CAP_PROP_ZOOM:
    case xreal_dshow::CV_CAP_PROP_EXPOSURE:
    case xreal_dshow::CV_CAP_PROP_IRIS:
    case xreal_dshow::CV_CAP_PROP_FOCUS:
        if (video_input_.getVideoSettingCamera(video_input_device_,video_input_.getCameraPropertyFromCV(property_id),min_value,max_value,stepping_delta,current_value,flags,defaultValue) ) return (double)current_value;

    }

    // unknown parameter or value not available
    return -1;
}

bool CameraDshow::setProperty( int property_id, double value ) {
    // image capture properties
    int width = -1, height = -1, fourcc = -1;
    bool handled = false;
    switch( property_id )
    {
    case xreal_dshow::CV_CAP_PROP_FRAME_WIDTH:
        width = xreal_dshow::cvRound(value);
        handled = true;
        break;

    case xreal_dshow::CV_CAP_PROP_FRAME_HEIGHT:
        height = xreal_dshow::cvRound(value);
        handled = true;
        break;

    case xreal_dshow::CV_CAP_PROP_FOURCC:
        fourcc = (int)(unsigned long)(value);
        if ( fourcc == -1 ) {
            // following cvCreateVideo usage will pop up caprturepindialog here if fourcc=-1
            // TODO - how to create a capture pin dialog
        }
        handled = true;
        break;

    case xreal_dshow::CV_CAP_PROP_FPS:
        int fps = xreal_dshow::cvRound(value);
        if (fps != video_input_.getFPS(video_input_device_))
        {
            video_input_.stopDevice(video_input_device_);
            video_input_.setIdealFramerate(video_input_device_,fps);
            if (info_.width > 0 && info_.height > 0)
                video_input_.setupDevice(video_input_device_, info_.width, info_.height);
            else
                video_input_.setupDevice(video_input_device_);
        }
        return video_input_.isDeviceSetup(video_input_device_);

    }

    if ( handled ) {
        // a stream setting
        if( width > 0 && height > 0 )
        {
            if( width != video_input_.getWidth(video_input_device_) || height != video_input_.getHeight(video_input_device_) )//|| fourcc != video_input_.getFourcc(video_input_device_) )
            {
                int fps = static_cast<int>(video_input_.getFPS(video_input_device_));
                video_input_.stopDevice(video_input_device_);
                video_input_.setIdealFramerate(video_input_device_, fps);
                video_input_.setupDeviceFourcc(video_input_device_, width, height, fourcc);
            }

            bool success = video_input_.isDeviceSetup(video_input_device_);
            if (success)
            {
                info_.width = width;
                info_.height = height;
                width = height = fourcc = -1;
            }
            return success;
        }
        return true;
    }

    // show video/camera filter dialog
    if ( property_id == xreal_dshow::CV_CAP_PROP_SETTINGS ) {
        video_input_.showSettingsWindow(video_input_device_);
        return true;
    }

    //video Filter properties
    switch( property_id )
    {
    case xreal_dshow::CV_CAP_PROP_BRIGHTNESS:
    case xreal_dshow::CV_CAP_PROP_CONTRAST:
    case xreal_dshow::CV_CAP_PROP_HUE:
    case xreal_dshow::CV_CAP_PROP_SATURATION:
    case xreal_dshow::CV_CAP_PROP_SHARPNESS:
    case xreal_dshow::CV_CAP_PROP_GAMMA:
    case xreal_dshow::CV_CAP_PROP_MONOCROME:
    case xreal_dshow::CV_CAP_PROP_WHITE_BALANCE_BLUE_U:
    case xreal_dshow::CV_CAP_PROP_BACKLIGHT:
    case xreal_dshow::CV_CAP_PROP_GAIN:
        return video_input_.setVideoSettingFilter(video_input_device_,video_input_.getVideoPropertyFromCV(property_id),(long)value);
    }

    //camera properties
    switch( property_id )
    {
    case xreal_dshow::CV_CAP_PROP_PAN:
    case xreal_dshow::CV_CAP_PROP_TILT:
    case xreal_dshow::CV_CAP_PROP_ROLL:
    case xreal_dshow::CV_CAP_PROP_ZOOM:
    case xreal_dshow::CV_CAP_PROP_EXPOSURE:
    case xreal_dshow::CV_CAP_PROP_IRIS:
    case xreal_dshow::CV_CAP_PROP_FOCUS:
        return video_input_.setVideoSettingCamera(video_input_device_,video_input_.getCameraPropertyFromCV(property_id),(long)value);
    }

    return false;
}

#endif