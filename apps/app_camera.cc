#include <puma/util/log.h>
#include <puma/env/system_config.h>

#if !defined(PUMA_SYSTEM_ANDROID)
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#endif

#include <libyuv.h>
using namespace libyuv;

#include <puma/provider/grayscale_camera_plugin.h>
#include "nonLockBuffer.h"
#include <vector>

#if !defined(PUMA_SYSTEM_ANDROID)
ConcurrentBufferNoLock<cv::Mat> image_buffer(30);
#endif
using namespace puma::plugin;


#define SHOW_COMBINED_IMAGE

#ifdef SHOW_COMBINED_IMAGE
NRGrayscaleCameraFrameData last_gray_camera_frame{};
std::vector<uint8_t>  one_image_buf;

NRGrayscaleCameraFrameData combined_frame{};
std::vector<uint8_t>  combine_image_buf;

bool CombineFrame(const NRGrayscaleCameraFrameData* frame)
{
    //PUMA_LOG_TRACE("CombineFrame camera_id={},timestamp_raw={}",frame->cameras[0].camera_id,frame->cameras[0].exposure_start_time_device);
    if (last_gray_camera_frame.data == nullptr) {
        last_gray_camera_frame = *frame;
        one_image_buf.resize(last_gray_camera_frame.data_bytes);
        CopyPlane((const uint8_t*)frame->data, frame->data_bytes, one_image_buf.data(), frame->data_bytes, frame->data_bytes, 1);
        last_gray_camera_frame.data = one_image_buf.data();
        return false;
    }
    else
    {
        int64_t delta_time_ns = frame->cameras[0].exposure_start_time_device - last_gray_camera_frame.cameras[0].exposure_start_time_device;
        int64_t delta_time_ms =  delta_time_ns / 1e6;
        if (delta_time_ms < 1) {
            //is the same group image
            combined_frame = *frame;

            std::vector<NRGrayscaleCameraFrameData*> one_group_frame;

            if(last_gray_camera_frame.cameras[0].camera_id == NR_GRAYSCALE_CAMERA_ID_1) {
                one_group_frame.emplace_back(&last_gray_camera_frame);
                one_group_frame.emplace_back((NRGrayscaleCameraFrameData*)frame);
            }
            else {
                one_group_frame.emplace_back((NRGrayscaleCameraFrameData*)frame);
                one_group_frame.emplace_back(&last_gray_camera_frame);
            }
               
            uint32_t  total_width = 0;
            uint32_t  every_height = one_group_frame[0]->cameras[0].height;
            combined_frame.data_bytes = 0;
            for(uint32_t i=0; i< one_group_frame.size(); i++) {
                combined_frame.data_bytes += one_group_frame[i]->data_bytes;
                total_width += one_group_frame[i]->cameras[0].width;
                if(every_height != one_group_frame[i]->cameras[0].height) {
                    PUMA_LOG_FATAL("CombineFrame height not equal.{}!={}",every_height,one_group_frame[i]->cameras[0].height);
                    std::abort();
                }
            }
            combine_image_buf.resize(combined_frame.data_bytes);

            uint32_t copy_offset  = 0;
            for(uint32_t i=0; i< one_group_frame.size(); i++) { 
                combined_frame.cameras[i] =  one_group_frame[i]->cameras[0];
                combined_frame.cameras[i].offset = copy_offset;
                combined_frame.cameras[i].stride = total_width;
                CopyPlane((const uint8_t*)one_group_frame[i]->data, one_group_frame[i]->cameras[0].width,
                    combine_image_buf.data()+copy_offset, total_width,
                    one_group_frame[i]->cameras[0].width, one_group_frame[i]->cameras[0].height);
                copy_offset += one_group_frame[i]->cameras[0].width;
            }

            combined_frame.data = combine_image_buf.data();
            combined_frame.camera_count = one_group_frame.size();

            last_gray_camera_frame.data = nullptr;
            // frame = (const NRGrayscaleCameraFrameData*)&combined_frame;
        }
        else {
            last_gray_camera_frame = *frame;
            one_image_buf.resize(last_gray_camera_frame.data_bytes);
            CopyPlane((const uint8_t*)frame->data, frame->data_bytes, one_image_buf.data(), frame->data_bytes, frame->data_bytes, 1);
            last_gray_camera_frame.data = one_image_buf.data();
            PUMA_LOG_TRACE("CombineFrame delta_time_ms:{} not less than 1ms, maybe has lost frame!", delta_time_ms);
            return false;
        }
    }
    return true;
}
#endif

void GrayCameraDataNotify(const NRGrayscaleCameraFrameData * data, uint32_t data_size) {
    PUMA_LOG_INFO("GrayCameraDataNotify");

#if !defined(PUMA_SYSTEM_ANDROID)
    uint32_t present_width = data->cameras[0].width;
    uint32_t present_height =  data->cameras[0].height;

    bool need_show = true;
    const void * show_image_data = data->data;
    uint32_t show_image_data_size = data->data_bytes;
#ifdef SHOW_COMBINED_IMAGE
    if(data->camera_count == 1) {
        need_show = CombineFrame(data);
        if(need_show) {
            show_image_data = combined_frame.data;
            show_image_data_size = combined_frame.data_bytes;
            present_width *= 2;
        }
    }
#else
    if(data->camera_count == 1) {
        if(data->cameras[0].camera_id == NR_GRAYSCALE_CAMERA_ID_0) {
            need_show = true; //只显示左摄像头的图像
        }
        else {
            need_show = false;
        }
    }  
#endif // !SHOW_COMBINED_IMAGE

    cv::Mat *img_buffer = image_buffer.getWritableBuffer();
    if (img_buffer && need_show) {
        *img_buffer = cv::Mat(present_height, present_width, CV_8UC1);
        CopyPlane((const uint8_t*)show_image_data, show_image_data_size, img_buffer->data, show_image_data_size, show_image_data_size, 1);
        image_buffer.doneWriteBuffer();
        image_buffer.setValid(true);
    }
#endif
}


int main(int argc, char **argv) {
	PUMA_LOG_INFO("camera_test!");
    puma::Logger::GetInstance()->SetLogAllLevel(true);

    GrayscaleCameraPlugin::GetInstance()->SetFrameCallback(GrayCameraDataNotify);
    auto ret = GrayscaleCameraPlugin::GetInstance()->StartCamera();
    if(ret != NR_PLUGIN_RESULT_SUCCESS) {
        PUMA_LOG_ERROR("StartCamera failed. ret: {}", ret);
        return -1;
    } 

    PUMA_LOG_INFO("wait input(input q will exit)...");
    while(1) {

        int key = 0;
#if defined(PUMA_SYSTEM_ANDROID)
         key = getchar();
#else
        if (image_buffer.isValid()) {
            int index = image_buffer.getReadableARBufferIndex();
            cv::Mat image = *image_buffer.getBuffer(index);
            cv::imshow("image", image);
        }
        key = cv::waitKey(5) % 256;
#endif
        if (key == 'q') {
            GrayscaleCameraPlugin::GetInstance()->StopCamera();
            break;
        }
    }

	framework::util::log::Logger::shutdown();
	return 0;
}
