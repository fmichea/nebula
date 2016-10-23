#ifndef NEBULA_MEMORY_MMU_HH_
# define NEBULA_MEMORY_MMU_HH_

# include <fcntl.h>
# include <list>
# include <cstring>
# include <string>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

# include "watch.hh"
# include "access.hh"
# include "segments.hh"
# include "../logging.hh"

# include "mbcs/mbc.hh"
# include "mbcs/mbc1.hh"
# include "mbcs/mbc2.hh"
# include "mbcs/mbc3.hh"
# include "mbcs/mbc5.hh"
# include "mbcs/romonly.hh"

# include "registers/registers.hh"

enum class GBType { GB, CGB, SGB };

# define NINTENDO_LOGO_SIZE 0x30

static const char NINTENDO_LOGO[NINTENDO_LOGO_SIZE + 1] =
    "\xce\xed\x66\x66\xcc\x0d\x00\x0b\x03\x73\x00\x83\x00\x0c\x00\x0d"
    "\x00\x08\x11\x1f\x88\x89\x00\x0e\xdc\xcc\x6e\xe6\xdd\xdd\xd9\x99"
    "\xbb\xbb\x67\x63\x6e\x0e\xec\xcc\xdd\xdc\x99\x9f\xbb\xb9\x33\x3e";

class MMU {
public:
    MMU();
    virtual ~MMU();

    bool load_rom(std::string filename);
    void do_hdma();

    template<typename T> T read(uint16_t addr);
    template<typename T> void write(uint16_t addr, T value);

    bool memcpy(uint8_t* dst, uint16_t srcaddr, uint16_t size);

    void subscribe(uint16_t addr, WatchType type, WatchHook hook, void* data);
    void unsubscribe(uint16_t addr, WatchHook hook);

# define X2(RegType, Reg, Addr, Value) RegType Reg;
# define X1(Reg, Addr, Value) X2(RegisterProxy, Reg, Addr, Value)
# include "registers.def"
# undef X1
# undef X2

    RegisterProxy IE;

    bool            stopped;
    GBType          gb_type = GBType::GB;
    bool            hdma_active = false;

private:
    nebula::memory::segments::Segment* _get_segment(uint16_t addr);

    uint8_t* _real_byte_ptr(AccessType type, uint16_t addr, uint8_t value);

    uint8_t _read_byte_masking(uint16_t addr, uint8_t value);
    uint8_t _read_byte(uint16_t addr);

    uint8_t* _write_byte_ptr(uint16_t addr, uint8_t value);
    uint8_t _write_byte_masking(uint16_t addr, uint8_t* ptr, uint8_t value);
    void _write_byte(uint16_t addr, uint8_t value);

    bool load_mbc(uint8_t ct_type);
    bool load_rom_size(uint8_t val);
    bool load_ram_size(uint8_t val);
    bool reset_registers();

private:
    Watch _watch;

private:
    int             fd_;
    MBC*            mbc_;
    size_t          size_;
    std::string     target_;
    uint16_t        hdma_index_ = 0;
    uint16_t        hdma_length_ = 0;

# define X(Type, Name, Size, InitVal) \
    Type* Name;
# include "mmaps.def"
# undef X

    void trigger_watch_event(uint16_t addr, WatchType type, uint8_t curval, uint8_t newval);

    // GPU needs to access video memory directly
    friend class GPU;
};

# include "mmu.hxx"

#endif // !NEBULA_MEMORY_MMU_HH_
