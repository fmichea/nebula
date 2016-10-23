#include "romonly.hh"

namespace nms = nebula::memory::segments;

ROMOnly::ROMOnly() : MBC() {}

uint8_t* ROMOnly::real_byte_ptr(AccessType type, uint16_t addr, uint8_t UNUSED(value)) {
    switch (type) {
    case AccessType::READ:
        if (nms::ROM.contains_address(addr)) {
            return nms::ROM.ptr(addr);
        }
        return nullptr;

    case AccessType::WRITE:
        return nullptr;
    };
}
