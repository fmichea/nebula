#include <gtest/gtest.h>

#include "memory/segments.hh"

// FIXME: these tests rely a lot on the internals, maybe change it to be
// more abstract? Buffer checks etc should not be needed. It does not matter
// how the banks are represented in memory as long as it works.

namespace nms = nebula::memory::segments;

TEST(Segment, TestSegmentContainsAddr) {
    nms::Segment s(false, 0, 0xff);

    for (uint16_t addr = 0; addr < 0x100; ++addr) {
        ASSERT_TRUE(s.contains_address(0));
    }
    ASSERT_FALSE(s.contains_address(0x100));
}

TEST(Segment, TestSegmentPointerNoBanking) {
    nms::Segment s(false, 0, 0xff);

    uint8_t* buf = s.buffer();
    ASSERT_TRUE(s.ptr(0) == buf);
    ASSERT_TRUE(s.ptr(0x10) == (buf + 0x10));
}

TEST(Segment, TestSegmentPointerBankedAllInOne) {
    nms::Segment s(false, 0, 0xff, 2);

    // All addresses should be in the banked zone.
    for (uint16_t addr = 0; addr < 0x100; ++addr) {
        ASSERT_TRUE(s.contains_address(addr));
        ASSERT_TRUE(s.in_banked_zone(addr));
    }
    ASSERT_FALSE(s.contains_address(0x100));

    // Banked without a separate bank0 starts on bank 0 selected. We fetch a
    // pointer to 0x10.
    ASSERT_TRUE(s.bank() == 0);
    uint8_t* ptr1 = s.ptr(0x10);

    // We change the bank selector to bank 1 and get a pointer to 0x10.
    s.select_bank(1);
    ASSERT_TRUE(s.bank() == 1);
    uint8_t* ptr2 = s.ptr(0x10);

    // Since those two pointers are for two different banks, they should not be
    // the same. Internals say they will be one bank size appart.
    ASSERT_TRUE(ptr1 + 0x100 == ptr2);

    // We select bank 0 again, and get pointer to 0x10 again.
    s.select_bank(0);
    ptr2 = s.ptr(0x10);

    // Since ptr1 and ptr2 were both fetched when bank selector was 0, they
    // should point to the same byte.
    ASSERT_TRUE(ptr1 == ptr2);
}

TEST(Segment, TestSegmentPointerOnBankedWithSeparateBank0) {
    nms::Segment s(false, 0, 0x1ff, 3, true);

    // Banked with a separate bank0 starts with bank 1 selected.
    ASSERT_TRUE(s.bank() == 1);

    // Check that all addresses up to 0x1ff are indeed considered in this
    // segment, but no more. Also check that half of the segment is indeed
    // considered part of the banked zone.
    for (uint16_t addr = 0; addr < 0x100; ++addr) {
        ASSERT_TRUE(s.contains_address(addr));
        ASSERT_FALSE(s.in_banked_zone(addr));
    }
    for (uint16_t addr = 0x100; addr < 0x200; ++addr) {
        ASSERT_TRUE(s.contains_address(addr));
        ASSERT_TRUE(s.in_banked_zone(addr));
    }
    ASSERT_FALSE(s.contains_address(0x200));

    // Now switching bank and checking pointer stuff.
    uint8_t* ptr1 = s.ptr(0);
    uint8_t* ptr2 = s.ptr(0x100);

    ASSERT_TRUE(ptr1 + 0x100 == ptr2);

    // Check that changing bank changes what 0x100 points to.
    s.select_bank(2);
    uint8_t* ptr3 = s.ptr(0x100);
    ASSERT_TRUE(ptr2 != ptr3);
    ASSERT_TRUE(ptr1 + 0x200 == ptr3);

    // Check that it is possible to refer to bank 0 in the banked zone.
    s.select_bank(0);
    ASSERT_TRUE(ptr1 == s.ptr(0x100));
}
