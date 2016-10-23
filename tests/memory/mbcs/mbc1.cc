#include <gtest/gtest.h>

#include "memory/access.hh"
#include "memory/segments.hh"
#include "memory/mbcs/mbc1.hh"

namespace nms = nebula::memory::segments;

// The fixture for testing class Foo.
class MBC1Test : public ::testing::Test {
protected:
    virtual void TearDown() {
        nms::ROM.reset();
    }
};

TEST_F(MBC1Test, TestROMB0ReadOnlyBelow2000) {
    MBC1 mbc;
    uint8_t* ptr;
    // We should have ROM bank 1 selected at this point.
    ASSERT_TRUE(nms::ROM.bank() == 1);
    // Reading gives us a proper address to read.
    ptr = mbc.real_byte_ptr(AccessType::READ, 0x1000, 0);
    ASSERT_TRUE(ptr != nullptr);
    // Writing returns a nullptr meaning read-only.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x1000, 0x5);
    ASSERT_TRUE(ptr == nullptr);
    // The bank selector did not change due to the write attempt.
    ASSERT_TRUE(nms::ROM.bank() == 1);
}

TEST_F(MBC1Test, TestROMB0ReadOnlyAbove2000ButBankSelectorChanges) {
    MBC1 mbc;
    // We should have ROM bank 1 selected at this point.
    ASSERT_TRUE(nms::ROM.bank() == 1);
    // Reading gives us a proper address to read.
    uint8_t* ptr1 = mbc.real_byte_ptr(AccessType::READ, 0x5000, 0);
    ASSERT_TRUE(ptr1 != nullptr);
    // Writing returns a nullptr meaning read-only.
    uint8_t* ptr2 = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 5);
    ASSERT_TRUE(ptr2 == nullptr);
    // Bank selector for ROM changed at this point.
    ASSERT_TRUE(nms::ROM.bank() == 5);
    // Getting the read pointer again gives us another address.
    uint8_t* ptr3 = mbc.real_byte_ptr(AccessType::READ, 0x5000, 0);
    ASSERT_TRUE(ptr3 != nullptr);
    ASSERT_TRUE(ptr1 != ptr3);
}

TEST_F(MBC1Test, TestROMB0Writing0SelectsBank1) {
    uint8_t* ptr;
    MBC1 mbc;
    // We should have ROM bank 1 selected at this point.
    ASSERT_TRUE(nms::ROM.bank() == 1);
    // Writing returns a nullptr meaning read-only.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 5);
    ASSERT_TRUE(ptr == nullptr);
    // Bank selector for ROM changed at this point.
    ASSERT_TRUE(nms::ROM.bank() == 5);
    // Writing returns a nullptr meaning read-only. Also selects bank 1.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 0);
    ASSERT_TRUE(ptr == nullptr);
    ASSERT_TRUE(nms::ROM.bank() == 1);
    // Selecting bank 0x10 select bank 0x10.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 0x10);
    ASSERT_TRUE(nms::ROM.bank() == 0x10);
}

TEST_F(MBC1Test, TestROMB0BankSelectorZone1isLower5bits) {
    uint8_t* ptr;
    MBC1 mbc;
    // We should have ROM bank 1 selected at this point.
    ASSERT_TRUE(nms::ROM.bank() == 1);
    // Writing returns a nullptr meaning read-only.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 5);
    ASSERT_TRUE(ptr == nullptr);
    // Bank selector for ROM changed at this point.
    ASSERT_TRUE(nms::ROM.bank() == 5);
    // Writing returns a nullptr meaning read-only. Also selects bank 1.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 0);
    ASSERT_TRUE(ptr == nullptr);
    ASSERT_TRUE(nms::ROM.bank() == 1);
    // Selecting bank 0x10 select bank 0x10.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 0x10);
    ASSERT_TRUE(nms::ROM.bank() == 0x10);
    // Selecting bank 0xff selects bank 0x1f (only 5 lower bits used).
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 0xff);
    ASSERT_TRUE(nms::ROM.bank() == 0x1f);
}

TEST_F(MBC1Test, TestROMB0ROMRAMModeSelect) {
    uint8_t* ptr;
    MBC1 mbc;
    // Default mode is ROM banking.
    ASSERT_TRUE(mbc.mode() == MBC1Mode::ROM_BANKING_MODE);
    // Writing to 0x6000-0x7fff selects between each mode.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x7000, 0x1);
    ASSERT_TRUE(ptr == nullptr);
    ASSERT_TRUE(mbc.mode() == MBC1Mode::RAM_BANKING_MODE);
    // Only bit 1 is used to select.
    mbc.real_byte_ptr(AccessType::WRITE, 0x7000, 0x10);
    ASSERT_TRUE(mbc.mode() == MBC1Mode::ROM_BANKING_MODE);
}

TEST_F(MBC1Test, TestROMB0BankSelectorZone2isUpperBitROMOrRAM) {
    uint8_t* ptr;
    MBC1 mbc;
    // We should have ROM bank 1 selected at this point.
    ASSERT_TRUE(nms::ROM.bank() == 1);
    // Writing returns a nullptr meaning read-only.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x5000, 1);
    ASSERT_TRUE(ptr == nullptr);
    // Bank selector for ROM changed at this point. Upper 2 bits of ROM
    // selector have been modified.
    ASSERT_TRUE(nms::ROM.bank() == 0x21);
    // Writing returns a nullptr meaning read-only.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x3000, 0x3);
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x5000, 0xff);
    ASSERT_TRUE(ptr == nullptr);
    // Only 2 bits are used.
    ASSERT_TRUE(nms::ROM.bank() == 0x63);
    // Now we switch to RAM banking and test that too.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x7000, 0x1);
    ASSERT_TRUE(ptr == nullptr);
    // Now writing to zone 2 of selector changes RAM bank.
    uint8_t bank = nms::ROM.bank();
    ASSERT_TRUE(bank == 0x3);
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x5000, 0x2);
    ASSERT_TRUE(ptr == nullptr);
    ASSERT_TRUE(nms::ROM.bank() == bank);
    ASSERT_TRUE(nms::ERAM.bank() == 0x2);
    // Switching back to ROM bank resets ERAM to bank 0.
    ptr = mbc.real_byte_ptr(AccessType::WRITE, 0x7000, 0);
    ASSERT_TRUE(ptr == nullptr);
    ASSERT_TRUE(nms::ERAM.bank() == 0);
}

TEST_F(MBC1Test, TestERAMIsBothReadableAndWriteable) {
    MBC1 mbc;
    // Read pointer is available in ERAM.
    uint8_t* ptr1 = mbc.real_byte_ptr(AccessType::READ, 0xB000, 0);
    ASSERT_TRUE(ptr1 != nullptr);
    // Write pointer is also available in ERAM.
    uint8_t* ptr2 = mbc.real_byte_ptr(AccessType::WRITE, 0xB000, 0);
    ASSERT_TRUE(ptr2 != nullptr);
    // Pointers are the same for read and write.
    ASSERT_TRUE(ptr1 == ptr2);
}
