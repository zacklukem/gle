#ifndef GLE_LOGGING_HPP
#define GLE_LOGGING_HPP

#ifdef GLE_VERBOSE
#  include <cstdio>
#  include <gle/common.hpp>
#  ifndef GLE_LOG
#    define GLE_LOG(...) gle::__internal__::log(__VA_ARGS__)
#  endif

GLE_NAMESPACE_BEGIN

enum LogLevel { GLE_INFO, GLE_WARN, GLE_ERR };

namespace __internal__ {

#  if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wformat-security"
#  elif defined(__GNUC__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wformat-security"
#  endif

template <class... Args> inline void log(LogLevel level, Args... args) {
  char *log_output = new char[0x200];
  std::sprintf(log_output, args...);
  const char *level_text;
  switch (level) {
  case GLE_INFO:
    level_text = "INFO";
    break;
  case GLE_WARN:
    level_text = "WARN";
    break;
  case GLE_ERR:
    level_text = "ERR";
    break;
  }
  std::printf("GLE: %s: %s\n", level_text, log_output);
  delete[] log_output;
}

#  if defined(__clang__)
#    pragma clang diagnostic pop
#  elif defined(__GNUC__)
#    pragma GCC diagnostic pop
#  endif

} // namespace __internal__

GLE_NAMESPACE_END

#else
#  define GLE_LOG(...)
#endif

#endif // GLE_LOGGING_HPP
