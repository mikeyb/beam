#include "utility/io/reactor.h"
#define LOG_VERBOSE_ENABLED 0
#include "utility/logger.h"
#include <future>
#include <iostream>

using namespace beam;
using namespace beam::io;
using namespace std;

void reactor_start_stop() {
    Reactor::Ptr reactor = Reactor::create();

    auto f = std::async(
        std::launch::async,
        [reactor]() {
            this_thread::sleep_for(chrono::microseconds(300000));
            //usleep(300000);
            LOG_DEBUG() << "stopping reactor from foreign thread...";
            reactor->stop();
        }
    );

    LOG_DEBUG() << "starting reactor...";;
    reactor->run();
    LOG_DEBUG() << "reactor stopped";

    f.get();
}

void error_codes_test() {
    std::string unknown_descr = error_descr(ErrorCode(1005000));
    std::string str;
#define XX(code, _) \
    str = format_io_error("", "", 0, EC_ ## code); \
    LOG_VERBOSE() << str; \
    assert(str.find(unknown_descr) == string::npos);
    
    UV_ERRNO_MAP(XX)
#undef XX
}

int main() {
    LoggerConfig lc;
    lc.consoleLevel = LOG_LEVEL_VERBOSE;
    lc.flushLevel = LOG_LEVEL_VERBOSE;
    auto logger = Logger::create(lc);
    reactor_start_stop();
    error_codes_test();
}
