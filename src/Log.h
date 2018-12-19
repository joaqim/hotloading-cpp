#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // support for stdout colored logging
#include <spdlog/sinks/basic_file_sink.h>    // support for basic file logging
namespace Terrific {
	namespace Utility {
		class Log {
			public:
				static int init();

				inline static std::shared_ptr<spdlog::logger> &getCoreLogger() {return _coreLogger; }
				inline static std::shared_ptr<spdlog::logger> &getClientLogger() {return _clientLogger; }
			private:
				static std::shared_ptr<spdlog::logger> _coreLogger;
				static std::shared_ptr<spdlog::logger> _clientLogger;
		};
	}
}

#define TERRIFIC_(logger,name,...)                                      \
    if (::Terrific::Utility::Log::get##logger##Logger()->should_log(spdlog::level::name)) { \
      ::Terrific::Utility::Log::get##logger##Logger()->name("{}:{}: in function ::{}(): {}", __FILE__,  __LINE__, __FUNCTION__,fmt::format(__VA_ARGS__)); \
    }

#define TERRIFIC_INFO_(logger,name,...)                                 \
  if (::Terrific::Utility::Log::get##logger##Logger()->should_log(spdlog::level::name)) { \
    ::Terrific::Utility::Log::get##logger##Logger()->name("{}", fmt::format(__VA_ARGS__)); \
  }


#define TERRIFIC_INFO(...) TERRIFIC_INFO_(Client,info,__VA_ARGS__)
#define TERRIFIC_WARNING(...) TERRIFIC_(Client,warn,__VA_ARGS__)
#define TERRIFIC_ERROR(...) TERRIFIC_(Client,trace,__VA_ARGS__)
#define TERRIFIC_TRACE(...) TERRIFIC_(Client,trace,__VA_ARGS__)
#define TERRIFIC_CRITICAL(...) TERRIFIC_(Client,critical,__VA_ARGS__)


#ifndef NDEBUG
void __Log_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);

#define GET_MACRO(Expr, Msg) Expr, Msg

#define Assert(Expr, Msg)                    \
  __Log_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#define MY_ASSERT1(Expr)                        \
  __Log_Assert(#Expr, Expr, __FILE__, __LINE__)
#define MY_ASSERT2(Expr, Msg)                         \
  __Log_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)


#ifdef _MSC_VER // Microsoft compilers

#   define GET_ARG_COUNT(...)  INTERNAL_EXPAND_ARGS_PRIVATE(INTERNAL_ARGS_AUGMENTER(__VA_ARGS__))

#   define INTERNAL_ARGS_AUGMENTER(...) unused, __VA_ARGS__
#   define INTERNAL_EXPAND(x) x
#   define INTERNAL_EXPAND_ARGS_PRIVATE(...) INTERNAL_EXPAND(INTERNAL_GET_ARG_COUNT_PRIVATE(__VA_ARGS__, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#   define INTERNAL_GET_ARG_COUNT_PRIVATE(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

#else // Non-Microsoft compilers

#   define GET_ARG_COUNT(...) INTERNAL_GET_ARG_COUNT_PRIVATE(0, ## __VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#   define INTERNAL_GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

#endif

#define MY_ASSERT_CHOOSE_HELPER2(count) MY_ASSERT##count
#define MY_ASSERT_CHOOSE_HELPER1(count) MY_ASSERT_CHOOSE_HELPER2(count)
#define MY_ASSERT_CHOOSE_HELPER(count)  MY_ASSERT_CHOOSE_HELPER1(count)
#define Assert__(...)                                                     \
  GET_MACRO(MY_ASSERT_CHOOSE_HELPER(GET_ARG_COUNT(__VA_ARGS__)), (__VA_

#else
#define Assert(Expr, Msg) ;
#endif



#define TERRIFIC_CORE_INFO(...) TERRIFIC_INFO_(Core,info,__VA_ARGS__)
#define TERRIFIC_CORE_WARNING(...) TERRIFIC_INFO_(Core,warn,__VA_ARGS__)
#define TERRIFIC_CORE_ERROR(...) TERRIFIC_(Core,trace,__VA_ARGS__)
#define TERRIFIC_CORE_TRACE(...) TERRIFIC_(Core,trace,__VA_ARGS__)
#define TERRIFIC_CORE_CRITICAL(...) TERRIFIC_(Core,critical,__VA_ARGS__)

#if 0
#define TERRIFIC_CORE_TRACE(...) ::Terrific::Utility::Log::getCoreLogger()->trace(__VA_ARGS__)
#define TERRIFIC_CORE_INFO(...)  ::Terrific::Utility::Log::getCoreLogger()->info(__VA_ARGS__)
#define TERRIFIC_CORE_WARN(...)  ::Terrific::Utility::Log::getCoreLogger()->warn(__VA_ARGS__)
#define TERRIFIC_CORE_ERROR(...) ::Terrific::Utility::Log::getCoreLogger()->error(__VA_ARGS__)
#define TERRIFIC_CORE_CRITICAL(...) ::Terrific::Utility::Log::getCoreLogger()->critical(__VA_ARGS__)

//#define TERRIFIC_TRACE(...) ::Terrific::Utility::Log::getClientLogger()->trace(__VA_ARGS__)
#define TERRIFIC_INFO(...)  ::Terrific::Utility::Log::getClientLogger()->info(__VA_ARGS__)
#define TERRIFIC_WARN(...)  ::Terrific::Utility::Log::getClientLogger()->warn(__VA_ARGS__)
#define TERRIFIC_ERROR(...) ::Terrific::Utility::Log::getClientLogger()->error(__VA_ARGS__)
#define TERRIFIC_CRITICAL(...) ::Terrific::Utility::Log::getClientLogger()->critical(__VA_ARGS__)
#endif
