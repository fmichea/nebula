#include "mbc.hh"

MBC::MBC(void* rom)
{
    this->rom_ = (char*) rom;
    this->ram_ = (char*) malloc(0x20000 * sizeof (char));
}

MBC::~MBC()
{
    free(this->ram_);
}
