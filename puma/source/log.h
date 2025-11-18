#pragma once

#include <string>

#include <framework/util/log.h>

#include <framework/util/singleton.h>

namespace puma {

class Logger : public framework::util::Singleton<Logger> {
   public:
    bool IsLogAllLevel() const { return log_all_level_; }
    void SetLogAllLevel(bool value) { 
		log_all_level_ = value; 
		if (log_all_level_) {
			// framework default is info
			GetLogger()->set_level(framework::util::log::LogLevel::trace);
			GetLogger()->flush_on(framework::util::log::LogLevel::trace);
		}
	}
	framework::util::log::LoggerPtr GetLogger() { return framework::util::log::Logger::defaultLogger(section_); }
	void SetSection(const std::string &section) { section_ = section; }

   private:
    bool log_all_level_{false};
	std::string section_{"Puma"};
};


}  // namespace puma
 
#define PUMA_LOG_TRACE(format, ...) \
	if (puma::Logger::GetInstance()->IsLogAllLevel()) { \
		puma::Logger::GetInstance()->GetLogger()->trace(format, ##__VA_ARGS__);
#define PUMA_LOG_TRACE2(format1, format2, ...) \
{ \
	std::string format = format1; \
	format += format2; \
	if (puma::Logger::GetInstance()->IsLogAllLevel()) { \
		puma::Logger::GetInstance()->GetLogger()->trace(format.c_str(), ##__VA_ARGS__); \
} 
#define PUMA_LOG_DEBUG(format, ...) \
	if (puma::Logger::GetInstance()->IsLogAllLevel()) { \
		puma::Logger::GetInstance()->GetLogger()->debug(format, ##__VA_ARGS__);
#define PUMA_LOG_INFO(format, ...) \
    puma::Logger::GetInstance()->GetLogger()->info(format, ##__VA_ARGS__);
#define PUMA_LOG_WARN(format, ...) \
    puma::Logger::GetInstance()->GetLogger()->warn(format, ##__VA_ARGS__);
#define PUMA_LOG_ERROR(format, ...) \
    puma::Logger::GetInstance()->GetLogger()->error(format, ##__VA_ARGS__);
#define PUMA_LOG_FATAL(format, ...) \
    puma::Logger::GetInstance()->GetLogger()->fatal(format, ##__VA_ARGS__);
