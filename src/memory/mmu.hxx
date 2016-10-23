#ifndef NEBULA_MEMORY_MMU_HXX_
# define NEBULA_MEMORY_MMU_HXX_

template<> uint8_t MMU::read<uint8_t>(uint16_t addr);
template<> uint16_t MMU::read<uint16_t>(uint16_t addr);

template<> void MMU::write<uint8_t>(uint16_t addr, uint8_t value);
template<> void MMU::write<uint16_t>(uint16_t addr, uint16_t value);

#endif /* !NEBULA_MEMORY_MMU_HXX_ */
