#pragma once

#include <framework/util/os_time.h>
#include <framework/util/json.h>
#include <framework/util/util.h>
#include <puma/env/system_config.h>
#include <puma/util/log.h>

#include <fstream>
#include <string>
#include <functional>
#include <atomic>

#define UTIL_COPY(__DST__, __SRC__, __NAME__) \
	(__DST__)->__NAME__ = (__SRC__)->__NAME__;

#define UTIL_COPY_EX(__DST__, __SRC__, __NAME__) \
	(__DST__).__NAME__ = (__SRC__).__NAME__;


namespace puma {

constexpr uint32_t AMBIENT_LIGHT_LOG_FREQUENCY = 100;
constexpr uint32_t IMU_LOG_FREQUENCY = 10000;
constexpr uint32_t GRAYSCALE_LOG_FREQUENCY = 600;
constexpr uint32_t RGB_LOG_FREQUENCY = 600;
constexpr uint32_t VSYNC_LOG_FREQUENCY = 600;

inline uint64_t GetTimeNano() {
	return framework::util::FMonotonicGetNs();
}

inline uint64_t GetTimeMs() {
    return framework::util::FMonotonicGetMs();
}

inline uint64_t GetRealTimeNs() {
    return framework::util::FRealTimeGetNs();
}

inline uint64_t GetRealTimeMs() {
    return framework::util::FRealTimeGetMs();
}

inline std::string GetDebugConfig() {
#ifdef PUMA_SYSTEM_XRLINUX
    std::string path = "/usrdata/sdk_global.json";
#else
    std::string path = "./sdk_global.json";
#endif
    std::ifstream file(path);
    if (file.is_open()) {
        PUMA_LOG_INFO("Open debug.json");
        Json::Value json_root;
        Json::CharReaderBuilder json_builder;
        json_builder["collectComments"] = false;
        JSONCPP_STRING json_errs;
        if (parseFromStream(json_builder, file, &json_root, &json_errs)) {
            // save string
            std::stringstream buffer;
            file.clear();
            file.seekg(0);
            buffer << file.rdbuf();
            return buffer.str();
        } else {
            PUMA_LOG_ERROR("PARSE DEBUG JSON FILE ERROR!");
        }
    } else {
        PUMA_LOG_TRACE("DEBUG FILE {} NOT EXIST!", path.c_str());
    }
    return "";
}

template <typename T>
class RefCounter {
public:
    static RefCounter<T> * create(const T& data, std::function<void(T&)> cleanup_func, int initial_count = 1) {
        return new RefCounter<T>(data, cleanup_func, initial_count);
    }

    RefCounter(const RefCounter&) = delete;
    RefCounter& operator=(const RefCounter&) = delete;

    T& get() {
        return data_;
    }

    void add_ref(int32_t count = 1) {
        count_ += count;
    }

    void release(int32_t count = 1) {
        count_ -= count;
        if (count_ <= 0) {
            delete this;
        }
    }

private:
    RefCounter(const T& data, std::function<void(T&)> cleanup_func, int initial_count)
        : data_(data), cleanup_func_(cleanup_func), count_(initial_count) {
    }

    ~RefCounter() {
        if (cleanup_func_) {
            cleanup_func_(data_);
        }
    }

    T data_{};
    std::function<void(T&)> cleanup_func_{};
    std::atomic<int> count_{0};
};

class AverageCalculator {
public:
	explicit AverageCalculator(std::string name, uint64_t print_interval_ns = 2 * OS_NS_PER_SEC);
	void Update(int64_t candidate);
	void Clear();

private:
	void Print(uint64_t now);

private:
	std::string name_;
	uint64_t print_interval_ns_;

private:
	uint64_t samples_ever_since_ = 0;
	uint64_t samples_since_last_print_ = 0;
	uint64_t total_ever_since_ = 0;
	uint64_t total_since_last_print_ = 0;
	uint64_t last_print_ns_ = 0;
};

} // namespace puma
