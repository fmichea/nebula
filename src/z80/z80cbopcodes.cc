#include "z80cbopcodes.hh"

uint16_t rlc_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    uint8_t val = (reg >> 7) & 0x1;

    (void) mmu;
    reg = (reg << 1) | val;
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    regs.F.h.set(0);
    regs.F.cy.set(val);
    return P(2, 8);
}

uint16_t rl_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    uint8_t tmp = regs.F.cy.get();

    (void) mmu;
    regs.F.cy.set((reg >> 7) & 0x1);
    reg = (reg << 1) | tmp;
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    regs.F.h.set(0);
    return P(2, 8);
}

uint16_t sla_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    regs.F.cy.set((reg >> 7) & 0x1);
    reg <<= 1;
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    regs.F.h.set(0);
    return P(2, 8);
}

uint16_t rrc_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    regs.F.cy.set(reg & 0x1);
    reg = (regs.F.cy.get() << 7) | (reg >> 1);
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    regs.F.h.set(0);
    return P(2, 8);
}

uint16_t rr_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    uint8_t tmp = regs.F.cy.get();

    (void) mmu;
    regs.F.cy.set(reg & 0x1);
    reg = (reg >> 1) | (tmp << 7);
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    regs.F.n.set(0);
    regs.F.h.set(0);
    return P(2, 8);
}

uint16_t sra_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    regs.F.set(0);
    regs.F.cy.set(reg & 0x1);
    reg = (reg >> 1) | (reg & 0x80);
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t srl_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    regs.F.set(0);
    regs.F.cy.set(reg & 0x1);
    reg >>= 1;
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t swap_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg)
{
    (void) mmu;
    reg = ((reg & 0xf0) >> 4) | ((reg & 0xf) << 4);
    regs.F.set(0);
    regs.F.zf.set(reg == 0 ? 0x1 : 0x0);
    return P(2, 8);
}

uint16_t res_nb_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg, uint8_t bit)
{
    (void) mmu;
    (void) regs;
    reg &= (~0 ^ (0x1 << bit));
    return P(2, 8);
}

uint16_t set_nb_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg, uint8_t bit)
{
    (void) mmu;
    (void) regs;
    reg |= (0x1 << bit);
    return P(2, 8);
}

uint16_t bit_nb_1B_reg(MMU& mmu, Z80Registers& regs, uint8_t& reg, uint8_t bit)
{
    (void) mmu;
    (void) regs;
    regs.F.zf.set((reg >> bit) & 0x1 ? 0x0 : 0x1);
    regs.F.n.set(0);
    regs.F.h.set(1);
    return P(2, 8);
}

#define X1(FuncName, OpName, Reg)                           \
    uint16_t FuncName(MMU& mmu, Z80Registers& regs)         \
    {                                                       \
        return OpName(mmu, regs, Reg);                      \
    }
#define X2(FuncName, OpName, Reg, Bit)                      \
    uint16_t FuncName(MMU& mmu, Z80Registers& regs)         \
    {                                                       \
        return OpName(mmu, regs, Reg, Bit);                 \
    }
#define H1(FuncName, OpName)                                \
    uint16_t FuncName(MMU& mmu, Z80Registers& regs)         \
    {                                                       \
        uint8_t tmp = mmu.read<uint8_t>(regs.HL.get());     \
        uint16_t res = OpName(mmu, regs, tmp);              \
        mmu.write<uint8_t>(regs.HL.get(), tmp);             \
        return res + P(0, 8);                               \
    }
#define H2(FuncName, OpName, Bit)                           \
    uint16_t FuncName(MMU& mmu, Z80Registers& regs)         \
    {                                                       \
        uint8_t tmp = mmu.read<uint8_t>(regs.HL.get());     \
        uint16_t res = OpName(mmu, regs, tmp, Bit);         \
        mmu.write<uint8_t>(regs.HL.get(), tmp);             \
        return res + P(0, 8);                               \
    }
#include "cbopcodes.def"
#undef X1
#undef X2
#undef H1
#undef H2

op CBOPCODES[0x100] = {
    // 0x0x
    rlc_b, rlc_c, rlc_d, rlc_e, rlc_h, rlc_l, rlc_mhl, rlc_a,
    rrc_b, rrc_c, rrc_d, rrc_e, rrc_h, rrc_l, rrc_mhl, rrc_a,
    // 0x1x
    rl_b, rl_c, rl_d, rl_e, rl_h, rl_l, rl_mhl, rl_a,
    rr_b, rr_c, rr_d, rr_e, rr_h, rr_l, rr_mhl, rr_a,
    // 0x2x
    sla_b, sla_c, sla_d, sla_e, sla_h, sla_l, sla_mhl, sla_a,
    sra_b, sra_c, sra_d, sra_e, sra_h, sra_l, sra_mhl, sra_a,
    // 0x3x
    swap_b, swap_c, swap_d, swap_e, swap_h, swap_l, swap_mhl, swap_a,
    srl_b, srl_c, srl_d, srl_e, srl_h, srl_l, srl_mhl, srl_a,
    // 0x4x
    bit_0_b, bit_0_c, bit_0_d, bit_0_e, bit_0_h, bit_0_l, bit_0_mhl, bit_0_a,
    bit_1_b, bit_1_c, bit_1_d, bit_1_e, bit_1_h, bit_1_l, bit_1_mhl, bit_1_a,
    // 0x5x
    bit_2_b, bit_2_c, bit_2_d, bit_2_e, bit_2_h, bit_2_l, bit_2_mhl, bit_2_a,
    bit_3_b, bit_3_c, bit_3_d, bit_3_e, bit_3_h, bit_3_l, bit_3_mhl, bit_3_a,
    // 0x6x
    bit_4_b, bit_4_c, bit_4_d, bit_4_e, bit_4_h, bit_4_l, bit_4_mhl, bit_4_a,
    bit_5_b, bit_5_c, bit_5_d, bit_5_e, bit_5_h, bit_5_l, bit_5_mhl, bit_5_a,
    // 0x7x
    bit_6_b, bit_6_c, bit_6_d, bit_6_e, bit_6_h, bit_6_l, bit_6_mhl, bit_6_a,
    bit_7_b, bit_7_c, bit_7_d, bit_7_e, bit_7_h, bit_7_l, bit_7_mhl, bit_7_a,
    // 0x8x
    res_0_b, res_0_c, res_0_d, res_0_e, res_0_h, res_0_l, res_0_mhl, res_0_a,
    res_1_b, res_1_c, res_1_d, res_1_e, res_1_h, res_1_l, res_1_mhl, res_1_a,
    // 0x9x
    res_2_b, res_2_c, res_2_d, res_2_e, res_2_h, res_2_l, res_2_mhl, res_2_a,
    res_3_b, res_3_c, res_3_d, res_3_e, res_3_h, res_3_l, res_3_mhl, res_3_a,
    // 0xax
    res_4_b, res_4_c, res_4_d, res_4_e, res_4_h, res_4_l, res_4_mhl, res_4_a,
    res_5_b, res_5_c, res_5_d, res_5_e, res_5_h, res_5_l, res_5_mhl, res_5_a,
    // 0xbx
    res_6_b, res_6_c, res_6_d, res_6_e, res_6_h, res_6_l, res_6_mhl, res_6_a,
    res_7_b, res_7_c, res_7_d, res_7_e, res_7_h, res_7_l, res_7_mhl, res_7_a,
    // 0xcx
    set_0_b, set_0_c, set_0_d, set_0_e, set_0_h, set_0_l, set_0_mhl, set_0_a,
    set_1_b, set_1_c, set_1_d, set_1_e, set_1_h, set_1_l, set_1_mhl, set_1_a,
    // 0xdx
    set_2_b, set_2_c, set_2_d, set_2_e, set_2_h, set_2_l, set_2_mhl, set_2_a,
    set_3_b, set_3_c, set_3_d, set_3_e, set_3_h, set_3_l, set_3_mhl, set_3_a,
    // 0xex
    set_4_b, set_4_c, set_4_d, set_4_e, set_4_h, set_4_l, set_4_mhl, set_4_a,
    set_5_b, set_5_c, set_5_d, set_5_e, set_5_h, set_5_l, set_5_mhl, set_5_a,
    // 0xfx
    set_6_b, set_6_c, set_6_d, set_6_e, set_6_h, set_6_l, set_6_mhl, set_6_a,
    set_7_b, set_7_c, set_7_d, set_7_e, set_7_h, set_7_l, set_7_mhl, set_7_a,
};
