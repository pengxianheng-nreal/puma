#include <puma/source/log.h>

int main() {
	PUMA_LOG_TRACE("log_test!");
	PUMA_LOG_DEBUG("log_test!");
	PUMA_LOG_INFO("log_test!");
	PUMA_LOG_WARN("log_test!");
	PUMA_LOG_ERROR("log_test!");
	PUMA_LOG_FATAL("log_test!");
	PUMA_LOG_TRACE("log_test_after_set_level!");
	PUMA_LOG_INFO("log_test_after_set_level!");
	framework::util::log::Logger::shutdown();
	return 0;
}

