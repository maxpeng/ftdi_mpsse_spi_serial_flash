#include "dump_util.h"
#include <ostream>
#include <sstream>
#include "fmt/ostream.h"


std::string DumpUtil::hexlify(const char *buf, size_t size)
{
    std::ostringstream oss;

    size_t col;
    for (size_t i = 0; i < size; ++i) {
        col = i % 16;
        if (col == 8) {
            fmt::print(oss, "- ");
        }
        else if ((col == 0) && (i != 0)) {
            fmt::print(oss, "\n");
        }
        fmt::print(oss, "{:02x} ", (uint8_t)buf[i]);
    }
    return oss.str();
}


std::string DumpUtil::hexlify(std::string const &str)
{
    return DumpUtil::hexlify(str.data(), str.size());
}
