#include "romonly.hh"

ROMOnly::ROMOnly(void* rom)
    : MBC(rom)
{}

void* ROMOnly::read_address(uint16_t addr)
{
    return this->rom_ + addr;
}
void* ROMOnly::write_address(uint16_t addr, uint16_t value)
{
	(void) value;
    return this->rom_ + addr;
}
