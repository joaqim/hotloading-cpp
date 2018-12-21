#include "Log.h"

#include <iostream>

namespace Terrific {
	namespace Utility {

  bool Log::isInitialized = false;
  std::shared_ptr<spdlog::logger> Log::_coreLogger;
  std::shared_ptr<spdlog::logger> Log::_clientLogger;

  int Log::init() {
    //_bCoreEnabled = true;
    //_bClientEnabled = true;

    //			spdlog::set_pattern("%^[%L] %^[%T] %n: %v%$");
    //spdlog::set_pattern("%^%L: %^[%T] %n: %v%$");

    // gnu
    spdlog::set_pattern("%^%v%$");

    //      watcom
    //..\src\ctrl\lister.c(109): Error! E1009: Expecting ';' but found '{'
    //..\src\ctrl\lister.c(120): Warning! W201: Unreachable code
    //spdlog::set_pattern("%^%v:[%l] %$");

    try {
      _coreLogger = spdlog::stdout_color_mt("TERRIFIC");
    } catch (const spdlog::spdlog_ex &ex) {
      std::cout << "Terrific::Utility::Log::init() TERRIFIC logger failed to initialize: " << ex.what() << "\n"; 
      return EXIT_FAILURE;
    }

    try {
      //TODO: If verbose:
      _clientLogger = spdlog::stdout_color_mt("APP");
      //        _clientLogger = spdlog::basic_logger_mt("APP", "logs/app.log");
    } catch (const spdlog::spdlog_ex &ex) {
      std::cout << "Terrific::Utility::Log::init() APP logger failed to initialize: " << ex.what() << "\n"; 
    }

    _clientLogger->set_pattern("%^%L: %^[%T] %n:\t%v%$");

    _coreLogger->set_level(spdlog::level::trace);
    _clientLogger->set_level(spdlog::level::trace);

    TERRIFIC_CORE_INFO("Logger initialized");
    TERRIFIC_INFO("Logger initialized");
#if 0
    TERRIFIC_CORE_WARNING("Logger Warning");

    TERRIFIC_CORE_TRACE("Trace");
    TERRIFIC_CORE_ERROR("Logger Error");
    TERRIFIC_CORE_CRITICAL("Logger Critical");
#endif

    isInitialized = true;
    return EXIT_SUCCESS;
  }
  }
}

void __Log_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg="") {
  if (!expr) {
    auto c = &file[1]; // Remove one dot from ../ so emacs can find the file correctly

    TERRIFIC_CRITICAL(
        "Assert failed:\t{}\n"
        "Expected:\t{}\n"
        "{}: line {}: {}\n", msg, expr_str, c, line, msg);
    abort();
  }
}
