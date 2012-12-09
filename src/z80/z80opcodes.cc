#include "z80opcodes.hh"

uint16_t inc_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    reg += 1;
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    regs.F.h.set(!reg || (reg & 0xf) == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    return P(1, 4);
}

uint16_t inc_2B_reg(MMU& mmu, Z80Registers& regs, WordRegProxy& reg)
{
    (void) mmu;
    (void) regs;
    reg.set(reg.get() + 1);
    return P(1, 8);
}

uint16_t inc_2B_reg(MMU& mmu, Z80Registers& regs, uint16_t& reg)
{
    (void) mmu;
    (void) regs;
    reg = reg + 1;
    return P(1, 8);
}

uint16_t dec_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    reg -= 1;
    regs.F.n.set(1);
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    regs.F.h.set((reg & 0xf) == 0xf ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t dec_2B_reg(MMU& mmu, Z80Registers& regs, WordRegProxy& reg)
{
    (void) mmu;
    (void) regs;
    reg.set(reg.get() - 1);
    return P(1, 8);
}

uint16_t dec_2B_reg(MMU& mmu, Z80Registers& regs, uint16_t& reg)
{
    (void) mmu;
    (void) regs;
    reg = reg - 1;
    return P(1, 8);
}

uint16_t ld_1B_reg_d8(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    reg = mmu.read<uint8_t>(regs.PC + 1);
    return P(2, 8);
}

uint16_t ld_2B_reg_d16(MMU& mmu, Z80Registers& regs, WordRegProxy& reg)
{
    reg.set(mmu.read<uint16_t>(regs.PC + 1));
    return P(3, 12);
}

uint16_t ld_2B_reg_d16(MMU& mmu, Z80Registers& regs, uint16_t& reg)
{
    reg = mmu.read<uint16_t>(regs.PC + 1);
    return P(3, 12);
}

uint16_t ld_1B_reg_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& a, uint8_t& b)
{
    (void) mmu;
    (void) regs;
    a = b;
    return P(1, 4);
}

uint16_t ld_1B_reg_mhl(MMU& mmu, Z80Registers& regs, uint8_t& a)
{
    a = mmu.read<uint8_t>(regs.HL.get());
    return P(1, 8);
}

uint16_t ld_mhl_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& a)
{
    mmu.write<uint8_t>(regs.HL.get(), a);
    return P(1, 8);
}

uint16_t add_a_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    uint16_t tmp = regs.A + reg;
    uint8_t tmp_ = (regs.A & 0xf) + (reg & 0xf);

    (void) mmu;
    regs.A = tmp;
    regs.F.n.set(0);
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t and_a_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    regs.A &= reg;
    regs.F.set(0x20);
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t or_a_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    regs.A |= reg;
    regs.F.set(0x00);
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t xor_a_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    regs.A ^= reg;
    regs.F.set(0x00);
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t adc_a_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    uint16_t tmp = regs.A + reg + regs.F.cy.get();
    uint8_t tmp_ = (regs.A & 0xf) + (reg & 0xf) + regs.F.cy.get();

    (void) mmu;
    regs.A = tmp;
    regs.F.n.set(0);
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t sub_a_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    uint16_t tmp = regs.A - reg;
    uint8_t tmp_ = (regs.A & 0xf) - (reg & 0xf);

    (void) mmu;
    regs.A = tmp;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.n.set(1);
    regs.F.h.set((0xf & tmp_) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((0xff & tmp) ^ tmp ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t sbc_a_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    uint16_t tmp = regs.A - reg - regs.F.cy.get();
    uint8_t tmp_ = (regs.A & 0xf) - (reg & 0xf) - regs.F.cy.get();

    (void) mmu;
    regs.A = tmp;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.n.set(1);
    regs.F.h.set((0xf & tmp_) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((0xff & tmp) ^ tmp ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t cp_a_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    uint16_t tmp = regs.A - reg;
    uint8_t tmp_ = (regs.A & 0xf) - (reg & 0xf);

    (void) mmu;
    regs.F.zf.set((tmp & 0xff) == 0 ? 0x1 : 0x0);
    regs.F.n.set(0x1);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t pop_2B_reg(MMU& mmu, Z80Registers& regs, WordRegProxy& reg)
{
    reg.set(mmu.read<uint16_t>(regs.SP));
    regs.SP += 2;
    return P(1, 12);
}

uint16_t pop_2B_reg(MMU& mmu, Z80Registers& regs, uint16_t& reg)
{
    reg = mmu.read<uint16_t>(regs.SP);
    regs.SP += 2;
    return P(1, 12);
}

uint16_t push_2B_reg(MMU& mmu, Z80Registers& regs, WordRegProxy& reg)
{
    regs.SP -= 2;
    mmu.write<uint16_t>(regs.SP, reg.get());
    return P(1, 16);
}

uint16_t push_2B_reg(MMU& mmu, Z80Registers& regs, uint16_t& reg)
{
    regs.SP -= 2;
    mmu.write<uint16_t>(regs.SP, reg);
    return P(1, 16);
}

uint16_t rst_nn(MMU& mmu, Z80Registers& regs, uint16_t addr)
{
    logging::debug("Interrupt : %X", addr);
    regs.SP -= 2;
    mmu.write<uint16_t>(regs.SP, regs.PC + 1);
    regs.PC = addr;
    return P(0, 16);
}

uint16_t ret_if(MMU& mmu, Z80Registers& regs, uint8_t a)
{
    if (a)
    {
        regs.PC = mmu.read<uint16_t>(regs.SP);
        regs.SP += 2;
        return P(0, 20);
    }
    return P(1, 8);
}

uint16_t add_hl_2B_reg(MMU& mmu, Z80Registers& regs, WordRegProxy& reg)
{
    uint32_t tmp = regs.HL.get() + reg.get();
    uint32_t tmp_ = (regs.HL.get() & 0xfff) + (reg.get() & 0xfff);

    (void) mmu;
    regs.HL.set(tmp);
    regs.F.n.set(0x0);
    regs.F.h.set((0xfff & tmp_) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((0xffff & tmp) ^ tmp ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t add_hl_2B_reg(MMU& mmu, Z80Registers& regs, uint16_t& reg)
{
    uint32_t tmp = regs.HL.get() + reg;
    uint32_t tmp_ = (regs.HL.get() & 0xfff) + (reg & 0xfff);

    (void) mmu;
    regs.HL.set(tmp);
    regs.F.n.set(0x0);
    regs.F.h.set((0xfff & tmp_) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((0xffff & tmp) ^ tmp ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t nop(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    (void) regs;
    return P(1, 4);
}

uint16_t rlca(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.F.set(0x00);
    regs.F.cy.set((regs.A >> 7) & 0x1);
    regs.A <<= 1;
    regs.A |= regs.F.cy.get();
    return P(1, 4);
}

uint16_t stop(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    (void) regs;
    // FIXME
    return P(1, 4);
}

uint16_t rla(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = regs.F.cy.get();

    (void) mmu;
    regs.F.set(0x00);
    regs.F.cy.set((regs.A >> 7) & 0x1);
    regs.A = (regs.A << 0x1) | val;
    return P(1, 4);
}

uint16_t rrca(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.F.set(0x00);
    regs.F.cy.set(regs.A & 0x1);
    regs.A >>= 1;
    regs.A |= (regs.F.cy.get() << 7);
    return P(1, 4);
}

uint16_t rra(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = regs.F.cy.get();

    (void) mmu;
    regs.F.set(0x00);
    regs.F.cy.set(regs.A & 0x1);
    regs.A = (regs.A >> 1) | (val << 7);
    return P(1, 4);
}

uint16_t cpl(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.A ^= 0xff;
    regs.F.n.set(1);
    regs.F.h.set(1);
    return P(1, 4);
}

uint16_t ccf(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.F.n.set(0);
    regs.F.h.set(0);
    regs.F.cy.set(regs.F.cy.get() ^ 0x1);
    return P(1, 4);
}

uint16_t ldh_a8_a(MMU& mmu, Z80Registers& regs)
{
    mmu.write<uint8_t>(0xff00 + mmu.read<uint8_t>(regs.PC + 1), regs.A);
    return P(2, 12);
}

uint16_t ld_ma16_a(MMU& mmu, Z80Registers& regs)
{
    mmu.write<uint8_t>(mmu.read<uint16_t>(regs.PC + 1), regs.A);
    return P(3, 16);
}

uint16_t ldh_a_a8(MMU& mmu, Z80Registers& regs)
{
    regs.A = mmu.read<uint8_t>(0xff00 + mmu.read<uint8_t>(regs.PC + 1));
    return P(2, 12);
}

uint16_t ld_a_ma16(MMU& mmu, Z80Registers& regs)
{
    regs.A = mmu.read<uint8_t>(mmu.read<uint16_t>(regs.PC + 1));
    return P(3, 16);
}

uint16_t call_if_a16(MMU& mmu, Z80Registers& regs, uint8_t value)
{
    if (value)
    {
        regs.SP -= 2;
        mmu.write<uint16_t>(regs.SP, regs.PC + 3);
        regs.PC = mmu.read<uint16_t>(regs.PC + 1);
        return P(0, 24);
    }
    return P(3, 12);
}

uint16_t call_a16(MMU& mmu, Z80Registers& regs)
{
    return call_if_a16(mmu, regs, 0x1);
}

uint16_t jump_if_a16(MMU& mmu, Z80Registers& regs, uint8_t value)
{
    if (value)
    {
        regs.PC = mmu.read<uint16_t>(regs.PC + 1);
        return P(0, 16);
    }
    return P(3, 12);
}

uint16_t jump_a16(MMU& mmu, Z80Registers& regs)
{
    return jump_if_a16(mmu, regs, 0x1);
}

uint16_t jump_mhl(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.PC = regs.HL.get();
    return P(0, 8);
}

uint16_t jr_if(MMU& mmu, Z80Registers& regs, uint8_t val)
{
    if (val)
    {
        regs.PC += (int8_t) mmu.read<uint8_t>(regs.PC + 1);
        return P(2, 12);
    }
    return P(2, 8);
}

uint16_t jr(MMU& mmu, Z80Registers& regs)
{
    return jr_if(mmu, regs, 0x1);
}

uint16_t ld_a_maddr(MMU& mmu, Z80Registers& regs, uint16_t addr)
{
    regs.A = mmu.read<uint8_t>(addr);
    return P(1, 8);
}

uint16_t ld_a_mbc(MMU& mmu, Z80Registers& regs)
{
    return ld_a_maddr(mmu, regs, regs.BC.get());
}

uint16_t ld_a_mde(MMU& mmu, Z80Registers& regs)
{
    return ld_a_maddr(mmu, regs, regs.DE.get());
}

uint16_t ld_a_mhlp(MMU& mmu, Z80Registers& regs)
{
    uint16_t res = ld_a_maddr(mmu, regs, regs.HL.get());
    regs.HL.set(regs.HL.get() + 1);
    return res;
}

uint16_t ld_a_mhlm(MMU& mmu, Z80Registers& regs)
{
    uint16_t res = ld_a_maddr(mmu, regs, regs.HL.get());
    regs.HL.set(regs.HL.get() - 1);
    return res;
}

uint16_t ld_maddr_a(MMU& mmu, Z80Registers& regs, uint16_t addr)
{
    mmu.write<uint8_t>(addr, regs.A);
    return P(1, 8);
}

uint16_t ld_mbc_a(MMU& mmu, Z80Registers& regs)
{
    return ld_maddr_a(mmu, regs, regs.BC.get());
}

uint16_t ld_mde_a(MMU& mmu, Z80Registers& regs)
{
    return ld_maddr_a(mmu, regs, regs.DE.get());
}

uint16_t ld_mhlp_a(MMU& mmu, Z80Registers& regs)
{
    uint16_t res = ld_maddr_a(mmu, regs, regs.HL.get());
    regs.HL.set(regs.HL.get() + 1);
    return res;
}

uint16_t ld_mhlm_a(MMU& mmu, Z80Registers& regs)
{
    uint16_t res = ld_maddr_a(mmu, regs, regs.HL.get());
    regs.HL.set(regs.HL.get() - 1);
    return res;
}

uint16_t scf(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.F.h.set(0);
    regs.F.n.set(0);
    regs.F.cy.set(1);
    return P(1, 4);
}

uint16_t di(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.IME = false;
    return P(1, 4);
}

uint16_t ei(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.IME = true;
    return P(1, 4);
}

uint16_t daa(MMU& mmu, Z80Registers& regs)
{
    uint16_t a = regs.A;

    (void) mmu;
    if (regs.F.n.get())
    {
        if (regs.F.h.get())
            a = (a - 6) & 0xff;
        if (regs.F.cy.get())
            a -= 0x60;
    }
    else
    {
        if (regs.F.h.get() || 9 < (a & 0xf))
            a += 0x06;
        if (regs.F.cy.get() || 0x9f < a)
            a += 0x60;
    }
    regs.F.h.set(0);
    if (0xff < a)
        regs.F.cy.set(1);
    regs.A = (uint8_t) a;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(1, 4);
}

uint16_t inc_mhl(MMU& mmu, Z80Registers& regs)
{
    uint8_t tmp = mmu.read<uint8_t>(regs.HL.get());

    mmu.write<uint8_t>(regs.HL.get(), tmp + 1);
    regs.F.n.set(0);
    regs.F.h.set((0xf & tmp) == 0xf ? 0x1 : 0x0);
    regs.F.zf.set(tmp == 0xff ? 0x1 : 0x0);
    return P(1, 12);
}

uint16_t dec_mhl(MMU& mmu, Z80Registers& regs)
{
    uint8_t tmp = mmu.read<uint8_t>(regs.HL.get()) - 1;

    mmu.write<uint8_t>(regs.HL.get(), tmp);
    regs.F.n.set(1);
    regs.F.h.set((tmp & 0xf) == 0xf ? 0x1 : 0x0);
    regs.F.zf.set(tmp == 0 ? 0x1 : 0x0);
    return P(1, 12);
}

uint16_t ld_mhl_d8(MMU& mmu, Z80Registers& regs)
{
    mmu.write<uint8_t>(regs.HL.get(), mmu.read<uint8_t>(regs.PC + 1));
    return P(2, 12);
}

uint16_t halt(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.halt_mode = true;
    return P(1, 4);
}

uint16_t ld_ma16_sp(MMU& mmu, Z80Registers& regs)
{
    mmu.write<uint16_t>(mmu.read<uint16_t>(regs.PC + 1), regs.SP);
    return P(3, 20);
}

uint16_t add_a_mhl(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.HL.get());
    uint16_t tmp = regs.A + val;
    uint8_t tmp_ = (regs.A & 0xf) + (val & 0xf);

    regs.A = tmp;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t adc_a_mhl(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.HL.get());
    uint16_t tmp = regs.A + val + regs.F.cy.get();
    uint8_t tmp_ = (regs.A & 0xf) + (val & 0xf) + regs.F.cy.get();

    regs.A = tmp;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t sub_a_mhl(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.HL.get());
    uint32_t tmp = regs.A - val;
    uint16_t tmp_ = (regs.A & 0xf) - (val & 0xf);

    regs.A = tmp;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.n.set(1);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t sbc_a_mhl(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.HL.get());
    uint16_t tmp = regs.A - val - regs.F.cy.get();
    uint8_t tmp_ = (regs.A & 0xf) - (val & 0xf) - regs.F.cy.get();

    regs.A = tmp;
    regs.F.n.set(1);
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t and_a_mhl(MMU& mmu, Z80Registers& regs)
{
    regs.F.set(0x20);
    regs.A &= mmu.read<uint8_t>(regs.HL.get());
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t xor_a_mhl(MMU& mmu, Z80Registers& regs)
{
    regs.F.set(0x00);
    regs.A ^= mmu.read<uint8_t>(regs.HL.get());
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t or_a_mhl(MMU& mmu, Z80Registers& regs)
{
    regs.F.set(0x00);
    regs.A |= mmu.read<uint8_t>(regs.HL.get());
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t cp_mhl(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.HL.get());
    uint16_t tmp = regs.A - val;
    uint8_t tmp_ = (regs.A & 0xf) - (val & 0xf);

    //regs.A = (uint8_t) tmp; TODO: check this....
    regs.F.n.set(1);
    regs.F.zf.set((tmp & 0xff) == 0 ? 0x1 : 0x0);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(1, 8);
}

uint16_t add_a_d8(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.PC + 1);
    uint16_t tmp = regs.A + val;
    uint8_t tmp_ = (regs.A & 0xf) + (val & 0xf);

    regs.A = tmp;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t ret(MMU& mmu, Z80Registers& regs)
{
    regs.PC = mmu.read<uint16_t>(regs.SP);
    regs.SP += 2;
    return P(0, 16);
}

uint16_t adc_a_d8(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.PC + 1);
    uint16_t tmp = regs.A + val + regs.F.cy.get();
    uint8_t tmp_ = (regs.A & 0xf) + (val & 0xf) + regs.F.cy.get();

    regs.A = tmp;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t sub_a_d8(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.PC + 1);
    uint16_t tmp = regs.A - val;
    uint8_t tmp_ = (regs.A & 0xf) - (val & 0xf);

    regs.A = tmp;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.n.set(1);
    regs.F.h.set((0xf & tmp_) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set(tmp ^ (tmp & 0xff) ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t sbc_a_d8(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.PC + 1);
    uint16_t tmp = regs.A - val - regs.F.cy.get();
    uint8_t tmp_ = (regs.A & 0xf) - (val & 0xf) - regs.F.cy.get();

    regs.A = tmp;
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    regs.F.n.set(1);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t ldh_ma8_a(MMU& mmu, Z80Registers& regs)
{
    mmu.write<uint8_t>(0xff00 + mmu.read<uint8_t>(regs.PC + 1), regs.A);
    return P(2, 12);
}

//uint16_t ld_ma16_a(MMU& mmu, Z80Registers& regs)
//{
//    mmu.write<uint8_t>(mmu.read<uint16_t>(regs.PC + 1), regs.A);
//    return P(3, 16);
//}

uint16_t ld_mc_a(MMU& mmu, Z80Registers& regs)
{
    mmu.write<uint8_t>(0xff00 + regs.C, regs.A);
    return P(1, 8);
}

uint16_t ld_a_mc(MMU& mmu, Z80Registers& regs)
{
    regs.A = mmu.read<uint8_t>(0xff00 + regs.C);
    return P(1, 8);
}

uint16_t and_a_d8(MMU& mmu, Z80Registers& regs)
{
    regs.F.set(0x20);
    regs.A &= mmu.read<uint8_t>(regs.PC + 1);
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t add_sp_r8(MMU& mmu, Z80Registers& regs)
{
    int8_t val = mmu.read<uint8_t>(regs.PC + 1);
    uint16_t res = regs.SP + val;

    regs.F.set(0);
    regs.F.h.set((res & 0xf) < (regs.SP & 0xf) ? 0x1 : 0x0);
    regs.F.cy.set((res & 0xff) < (regs.SP & 0xff) ? 0x1 : 0x0);
    regs.SP = res;
    return P(2, 16);
}

uint16_t ldh_a_ma8(MMU& mmu, Z80Registers& regs)
{
    regs.A = mmu.read<uint8_t>(0xff00 + mmu.read<uint8_t>(regs.PC + 1));
    return P(2, 12);
}

uint16_t xor_a_d8(MMU& mmu, Z80Registers& regs)
{
    regs.F.set(0);
    regs.A ^= mmu.read<uint8_t>(regs.PC + 1);
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t or_a_d8(MMU& mmu, Z80Registers& regs)
{
    regs.F.set(0);
    regs.A |= mmu.read<uint8_t>(regs.PC + 1);
    regs.F.zf.set(regs.A == 0 ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t ld_hl_sppr8(MMU& mmu, Z80Registers& regs)
{
    int8_t val = mmu.read<uint8_t>(regs.PC + 1);
    uint16_t res = regs.SP + val;

    regs.F.set(0);
    regs.F.h.set((res & 0xf) < (regs.SP & 0xf) ? 0x1 : 0x0);
    regs.F.cy.set((res & 0xff) < (regs.SP & 0xff) ? 0x1 : 0x0);
    regs.HL.set(res);
    return P(2, 12);
}

uint16_t ld_sp_hl(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    regs.SP = regs.HL.get();
    return P(1, 8);
}

uint16_t cp_a_d8(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.PC + 1);
    uint16_t tmp = regs.A - val;
    uint8_t tmp_ = (regs.A & 0xf) - (val & 0xf);

    regs.F.zf.set((tmp & 0xff) == 0 ? 0x1 : 0x0);
    regs.F.n.set(1);
    regs.F.h.set((tmp_ & 0xf) ^ tmp_ ? 0x1 : 0x0);
    regs.F.cy.set((tmp & 0xff) ^ tmp ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t reti(MMU& mmu, Z80Registers& regs)
{
    ei(mmu, regs);
    ret(mmu, regs);
    return P(0, 16);
}

uint16_t cb(MMU& mmu, Z80Registers& regs)
{
    uint8_t op = mmu.read<uint8_t>(regs.PC + 1);
    return CBOPCODES[op](mmu, regs);
}

#define X1(FuncName, OpName, Reg)                               \
    uint16_t FuncName(MMU& mmu, Z80Registers& regs)             \
    {                                                           \
        return OpName(mmu, regs, Reg);                          \
    }
#define X2(FuncName, OpName, Reg1, Reg2)                        \
    uint16_t FuncName(MMU& mmu, Z80Registers& regs)             \
    {                                                           \
        return OpName(mmu, regs, Reg1, Reg2);                   \
    }
#include "opcodes.def"
#undef X2
#undef X1

op OPCODES[0x100] = {
    // 0x0x
    nop, ld_bc_d16, ld_mbc_a, inc_bc_reg, inc_b_reg, dec_b_reg, ld_b_d8, rlca,
    ld_ma16_sp, add_hl_bc, ld_a_mbc, dec_bc_reg, inc_c_reg, dec_c_reg, ld_c_d8, rrca,
    // 0x1x
    stop, ld_de_d16, ld_mde_a, inc_de_reg, inc_d_reg, dec_d_reg, ld_d_d8, rla,
    jr, add_hl_de, ld_a_mde, dec_de_reg, inc_e_reg, dec_e_reg, ld_e_d8, rra,
    // 0x2x
    jr_nzf, ld_hl_d16, ld_mhlp_a, inc_hl_reg, inc_h_reg, dec_h_reg, ld_h_d8, daa,
    jr_zf, add_hl_hl, ld_a_mhlp, dec_hl_reg, inc_l_reg, dec_l_reg, ld_l_d8, cpl,
    // 0x3x
    jr_ncy, ld_sp_d16, ld_mhlm_a, inc_sp_reg, inc_mhl, dec_mhl, ld_mhl_d8, scf,
    jr_cy, add_hl_sp, ld_a_mhlm, dec_sp_reg, inc_a_reg, dec_a_reg, ld_a_d8, ccf,
    // 0x4x
    ld_b_b, ld_b_c, ld_b_d, ld_b_e, ld_b_h, ld_b_l, ld_b_mhl, ld_b_a,
    ld_c_b, ld_c_c, ld_c_d, ld_c_e, ld_c_h, ld_c_l, ld_c_mhl, ld_c_a,
    // 0x5x
    ld_d_b, ld_d_c, ld_d_d, ld_d_e, ld_d_h, ld_d_l, ld_d_mhl, ld_d_a,
    ld_e_b, ld_e_c, ld_e_d, ld_e_e, ld_e_h, ld_e_l, ld_e_mhl, ld_e_a,
    // 0x6x
    ld_h_b, ld_h_c, ld_h_d, ld_h_e, ld_h_h, ld_h_l, ld_h_mhl, ld_h_a,
    ld_l_b, ld_l_c, ld_l_d, ld_l_e, ld_l_h, ld_l_l, ld_l_mhl, ld_l_a,
    // 0x7x
    ld_mhl_b, ld_mhl_c, ld_mhl_d, ld_mhl_e, ld_mhl_h, ld_mhl_l, halt, ld_mhl_a,
    ld_a_b, ld_a_c, ld_a_d, ld_a_e, ld_a_h, ld_a_l, ld_a_mhl, ld_a_a,
    // 0x8x
    add_a_b, add_a_c, add_a_d, add_a_e, add_a_h, add_a_l, add_a_mhl, add_a_a,
    adc_a_b, adc_a_c, adc_a_d, adc_a_e, adc_a_h, adc_a_l, adc_a_mhl, adc_a_a,
    // 0x9x
    sub_a_b, sub_a_c, sub_a_d, sub_a_e, sub_a_h, sub_a_l, sub_a_mhl, sub_a_a,
    sbc_a_b, sbc_a_c, sbc_a_d, sbc_a_e, sbc_a_h, sbc_a_l, sbc_a_mhl, sbc_a_a,
    // 0xAx
    and_a_b, and_a_c, and_a_d, and_a_e, and_a_h, and_a_l, and_a_mhl, and_a_a,
    xor_a_b, xor_a_c, xor_a_d, xor_a_e, xor_a_h, xor_a_l, xor_a_mhl, xor_a_a,
    // 0xBx
    or_a_b, or_a_c, or_a_d, or_a_e, or_a_h, or_a_l, or_a_mhl, or_a_a,
    cp_a_b, cp_a_c, cp_a_d, cp_a_e, cp_a_h, cp_a_l, cp_mhl, cp_a_a,
    // 0xCx
    ret_nzf, pop_bc, jump_nzf_a16, jump_a16, call_nzf_a16, push_bc, add_a_d8, rst_00h,
    ret_zf, ret, jump_zf_a16, cb, call_zf_a16, call_a16, adc_a_d8, rst_08h,
    // 0xDx
    ret_ncy, pop_de, jump_ncy_a16, 0, call_ncy_a16, push_de, sub_a_d8, rst_10h,
    ret_cy, reti, jump_cy_a16, 0, call_cy_a16, 0, sbc_a_d8, rst_18h,
    // 0xEx
    ldh_ma8_a, pop_hl, ld_mc_a, 0, 0, push_hl, and_a_d8, rst_20h,
    add_sp_r8, jump_mhl, ld_ma16_a, 0, 0, 0, xor_a_d8, rst_28h,
    // 0xFx
    ldh_a_ma8, pop_af, ld_a_mc, di, 0, push_af, or_a_d8, rst_30h, ld_hl_sppr8,
    ld_sp_hl, ld_a_ma16, ei, 0, 0, cp_a_d8, rst_38h
};
