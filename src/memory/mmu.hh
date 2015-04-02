#ifndef NEBULA_MEMORY_MMU_HH_
# define NEBULA_MEMORY_MMU_HH_

# include <fcntl.h>
# include <list>
# include <string.h>
# include <string>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

# include "../logging.hh"

# include "mbcs/mbc.hh"
# include "mbcs/mbc1.hh"
# include "mbcs/mbc2.hh"
# include "mbcs/mbc3.hh"
# include "mbcs/mbc5.hh"
# include "mbcs/romonly.hh"

# include "registers/registers.hh"

# define GB_ADDR_SPACE 0x10000

enum class GBType { GB, CGB, SGB };
enum class WatchType { RO, WO, RW };

typedef void (*WatchHook)(void*, uint16_t);

class MMU {
public:
    MMU();
    ~MMU();

    bool load_rom(std::string filename);
    void do_hdma();

    template<typename T> T read(uint16_t addr, bool twe=true);
    template<typename T> void write(uint16_t addr, T value, bool twe=true);

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
    bool load_mbc(uint8_t ct_type);
    bool load_rom_size(uint8_t val);
    bool load_ram_size(uint8_t val);
    bool reset_registers();

    char            title_[0x18];
    int             fd_;
    MBC*            mbc_;
    size_t          size_;
    std::string     target_;
    uint16_t        hdma_index_ = 0;
    uint16_t        hdma_length_ = 0;

# define X(Name, Size)      \
    uint8_t*        Name;
# include "mmap.def"
# undef X

    // Type to store hooks on events, this is used to keep track of them and
    // allow easy subscribe/unsubscribe. Note that subscribe/unsubscribe are
    // not optimized for being fast yet.
    typedef struct {
        WatchType type;
        WatchHook hook;
        void* data;
    } WatchRef;

    class WatchRefFinder {
    public:
        WatchRefFinder(WatchHook hook)
            : hook_ (hook)
        {}

        bool operator () (const WatchRef* ref) {
            return ref->hook == this->hook_;
        }

    private:
        WatchHook hook_;
    };

    typedef std::list<WatchRef*> WatchRefList;

    WatchRefList watchref_lists_[GB_ADDR_SPACE];

    void trigger_watch_event(bool twe, uint16_t addr, WatchType type);

    // GPU needs to access video memory directly
    friend class GPU;
};

# include "mmu.hxx"

#endif // !NEBULA_MEMORY_MMU_HH_
