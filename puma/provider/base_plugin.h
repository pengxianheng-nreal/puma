#pragma once

#include <puma/interface/common/nr_plugin_generic.h>
#include <framework/util/android_globals.h>
#include <framework/util/plugin_util.h>
#include <puma/env/system_config.h>
#include <puma/util/log.h>
#include <framework/util/ringbuffer.h>

#include <vector>
#include <memory>

using namespace framework::util;

namespace puma::plugin {

constexpr uint32_t kFrameBufferSize = 0;
constexpr uint32_t kFrameBufferItemSize = 4*1024*1024;
template<uint32_t ITEM_SIZE = kFrameBufferItemSize>
struct DataBuffer {
    unsigned char * buffer;
    uint32_t size;
    DataBuffer() : buffer(nullptr), size(0) {
        buffer = new unsigned char [ITEM_SIZE];
    }
    ~DataBuffer() {
        delete[] buffer;
    }
};

template <typename Value>
class RingBufferRWLocker: public RingBufferRW<Value> {
public:
    enum {
        WRITE_WAIT_TIME = 2, // 2s
    };
    RingBufferRWLocker(int32_t buffer_size)
            : RingBufferRW<Value>(buffer_size) {}
    Value* GetWritableBuffer() {
        if ((int32_t)(this->cur_write_index_ - this->cur_read_index_) >= this->buffer_size_) {
            std::unique_lock<std::mutex> guard(write_mutex_);
            if ((int32_t)(this->cur_write_index_ - this->cur_read_index_) >= this->buffer_size_) {
                while (!write_condition_.wait_for(
                        guard, std::chrono::seconds(WRITE_WAIT_TIME), [this] {
                            return (int32_t)(this->cur_write_index_ - this->cur_read_index_) < this->buffer_size_;
                        })) {
                    PUMA_LOG_WARN("RingBufferRWLocker is full, wait for {} seconds", WRITE_WAIT_TIME);
                }
            }
        }

        return RingBufferRW<Value>::GetWritableBuffer();
    }
    void DoneWriteBuffer() {
        RingBufferRW<Value>::DoneWriteBuffer();
        read_condition_.notify_one();
    }
    Value* GetReadableBuffer() {
        if (this->cur_read_index_ >= this->cur_write_index_) {
            std::unique_lock<std::mutex> guard(read_mutex_);
            read_condition_.wait(guard, [this] {
                return this->cur_read_index_ < this->cur_write_index_;
            });
        }
        return RingBufferRW<Value>::GetReadableBuffer();
    }
    void DoneReadBuffer() {
        RingBufferRW<Value>::DoneReadBuffer();
        write_condition_.notify_all();
    }
    void ResetReadableBuffer() {
        this->cur_read_index_ = (int64_t)this->cur_write_index_;
        is_reading_ = true;
    }
    void WakeupBufferForFinishRead() {
        this->cur_read_index_ = this->cur_write_index_ - 1;
        read_condition_.notify_one();
        is_reading_ = false;
        write_condition_.notify_all();
    }

protected:
    std::mutex read_mutex_;
    std::condition_variable read_condition_;
    std::mutex write_mutex_;
    std::condition_variable write_condition_;
    std::atomic<bool> is_reading_{false};
};

NR_DECLARE_INTERFACE(NREmptySubmitInterface) {
};

template <typename InterfaceType>
class BasePlugin {
public:
    explicit BasePlugin() = default;
    virtual ~BasePlugin() = default;

    virtual bool InitInterface(NRInterfaces* interfaces) {
        if(interfaces == nullptr) {
            PUMA_LOG_ERROR("NRInterfaces is null!");
            return false;
        }

        generic_interface_ = framework::util::GetInterface<NRGenericInterface>(interfaces);
        interface_ = framework::util::GetInterface<InterfaceType>(interfaces);
        if (!interface_) {
            PUMA_LOG_ERROR("interface_ is null!");
            return false;
        }
        if (!generic_interface_) {
            PUMA_LOG_ERROR("generic interface is null!");
            return false;
        }

        return true;
    }

    /// interface
    virtual NRPluginResult Register( NRPluginHandle handle ) = 0;
    virtual NRPluginResult Unregister( NRPluginHandle handle ) = 0;
    virtual NRPluginResult Initialize( NRPluginHandle handle ) = 0;
    virtual NRPluginResult Start( NRPluginHandle handle ) = 0;
    virtual NRPluginResult Pause( NRPluginHandle handle ) = 0;
    virtual NRPluginResult Resume( NRPluginHandle handle ) = 0;
    virtual NRPluginResult Stop( NRPluginHandle handle ) = 0;
    virtual NRPluginResult Release( NRPluginHandle handle ) = 0;

    NRPluginHandle GetPluginHandle() { return handle_; }

    void SetAesKeyIv(const std::string& key, const std::string& iv) {
        aes_key_ = key;
        aes_iv_ = iv;
    }
    const std::string GetAesKey() const { return aes_key_; }
    const std::string GetAesIv() const { return aes_iv_; }

protected:
    /*
     * All log function must be executed after this function has been called.
     * Because android log require android environment.
     */
    void InitAndroidEnvironment();
public:
    virtual bool ParseGlobalConfig(const char *config, uint32_t size) { return true; }

protected:
    NRPluginHandle handle_{0};
    std::vector<void*> providers_{};
    std::unique_ptr<NRGenericInterface> generic_interface_{nullptr};
    std::unique_ptr<InterfaceType> interface_{nullptr};

    std::string aes_key_;
    std::string aes_iv_;
    bool is_running_{false};
};

template <typename InterfaceType>
void BasePlugin<InterfaceType>::InitAndroidEnvironment() {
#ifdef PUMA_SYSTEM_ANDROID
    _JavaVM* java_vm = nullptr;
    void* obj_activity = nullptr;
    void* extra_class_loader = nullptr;
    generic_interface_->GetActivityInfo(
            reinterpret_cast<void**>(&java_vm),
            reinterpret_cast<void**>(&obj_activity),
            reinterpret_cast<void**>(&extra_class_loader));

    if(framework::util::android::GetJavaVM() == nullptr) {
        framework::util::android::StoreJavaVM(java_vm);
    }

    if(framework::util::android::GetActivity() == nullptr) {
        framework::util::android::StoreActivity(obj_activity);
    }

    if(framework::util::android::GetExtraClassLoader() == nullptr) {
        framework::util::android::StoreExtraClassLoader(extra_class_loader);
    }
#endif
}

};// puma::plugin
