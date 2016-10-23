#include <gtest/gtest.h>

#include "memory/access.hh"
#include "memory/segments.hh"
#include "memory/mbcs/mbc2.hh"

namespace nms = nebula::memory::segments;

class MBC2Test : public ::testing::Test {
protected:
    virtual void TearDown() {
        nms::ROM.reset();
    }
};

TEST_F(MBC2Test, TestROMSelectorSupportsOnly16BanksNot0) {
    uint8_t* ptr;
    MBC2 mbc;
    // Default bank is bank 1.
    ASSERT_TRUE(nms::ROM.bank() == 1);
    // CHanging bank to bank 3 works.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 3);
    ASSERT_TRUE(ptr == nullptr);
    ASSERT_TRUE(nms::ROM.bank() == 3);
    // CHanging bank to bank 0xff ignores upper nibble.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 0xff);
    ASSERT_TRUE(ptr == nullptr);
    ASSERT_TRUE(nms::ROM.bank() == 0xf);
    // Changing bank to bank 0 selects bank 1.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 0);
    ASSERT_TRUE(ptr == nullptr);
    ASSERT_TRUE(nms::ROM.bank() == 1);
}
