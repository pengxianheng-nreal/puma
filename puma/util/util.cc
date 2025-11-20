#include <puma/util/util.h>
#include <puma/env/system_config.h>
#include <puma/util/log.h>

using namespace puma;
using namespace framework::util;

AverageCalculator::AverageCalculator(std::string name, uint64_t print_interval_ns)
:name_(name), print_interval_ns_(print_interval_ns)
{

}

void AverageCalculator::Clear() {
	samples_ever_since_ = 0;
	samples_since_last_print_ = 0;
	last_print_ns_ = 0;
	total_ever_since_ = 0;
	total_since_last_print_ = 0;
}

void AverageCalculator::Update(int64_t candidate){
	samples_ever_since_++;
	samples_since_last_print_++;
	total_ever_since_ += candidate;
	total_since_last_print_ += candidate;

	uint64_t now = FMonotonicGetNs();
	if (now - last_print_ns_ > print_interval_ns_)
	{
		Print(now);
	}
	return;
}

void AverageCalculator::Print(uint64_t now)
{
	PUMA_LOG_INFO("[{}] {:.2f} {:.2f}, {} {}", name_,(1.0 * total_ever_since_) / samples_ever_since_, (1.0 * total_since_last_print_) / samples_since_last_print_, samples_ever_since_, samples_since_last_print_);
	samples_since_last_print_ = 0;
	total_since_last_print_ = 0;
	last_print_ns_ = now;
}

