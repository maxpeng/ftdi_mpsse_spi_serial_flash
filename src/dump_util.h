#ifndef DUMP_UTIL_H
#define DUMP_UTIL_H

#include <string>


class DumpUtil {
public:
    static std::string hexlify(const char *buf, size_t size);
    static std::string hexlify(std::string const &str);
};


#endif // DUMP_UTIL_H
