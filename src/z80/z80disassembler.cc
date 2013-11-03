#include "z80disassembler.hh"

#define DISASS_LINE_SIZE 1024
#define HEADER_SIZE 16

char    disass_line[DISASS_LINE_SIZE];

void inc_reg(MMU& mmu, Z80Registers& regs, const char* reg)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "inc %s", reg);
}

void dec_reg(MMU& mmu, Z80Registers& regs, const char* reg)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "dec %s", reg);
}

void push_reg(MMU& mmu, Z80Registers& regs, const char* reg)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "push %s", reg);
}

void pop_reg(MMU& mmu, Z80Registers& regs, const char* reg)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "pop %s", reg);
}

void ld_a_mreg(MMU& mmu, Z80Registers& regs, const char* reg)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "ld %%a, (%s)", reg);
}

void ld_mreg_a(MMU& mmu, Z80Registers& regs, const char* reg)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "ld (%s), a", reg);
}

void add_hl_reg(MMU& mmu, Z80Registers& regs, const char* reg)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "add %%hl, %s", reg);
}

template<typename T>
void ld_reg_val(MMU& mmu, Z80Registers& regs, const char* reg)
{
    T val = mmu.read<T>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "ld %s, $0x%X", reg, val);
}

void op_mhl(MMU& mmu, Z80Registers& regs, const char* op)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "%s (%%hl)", op);
}

void op_a_d8(MMU& mmu, Z80Registers& regs, const char* op)
{
    uint8_t val = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "%s %%a, $0x%02x", op, val);
}

void uniq_operation(MMU& mmu, Z80Registers& regs, const char* op)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "%s", op); // Opcode with no parameter.
}

void call_if_a16(MMU& mmu, Z80Registers& regs, const char* flag)
{
    uint16_t addr = mmu.read<uint16_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "call %s, $0x%04x", flag, addr);
}

void jump_if_a16(MMU& mmu, Z80Registers& regs, const char* flag)
{
    uint16_t addr = mmu.read<uint16_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "jmp %s, $0x%04x", flag, addr);
}

void jr_if_a16(MMU& mmu, Z80Registers& regs, const char* flag)
{
    uint8_t off = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "jr %s, $0x%02x ($%d)", flag, off, (int8_t) off);
}

void ret_if(MMU& mmu, Z80Registers& regs, const char* flag)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "ret %s", flag);
}

void ld_reg_reg(MMU& mmu, Z80Registers& regs, const char* r1, const char* r2)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "ld %s, %s", r1, r2);
}

void op_a_reg(MMU& mmu, Z80Registers& regs, const char* op, const char* reg)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "%s %%a, %s", op, reg);
}

void rst_nn(MMU& mmu, Z80Registers& regs, uint8_t nn)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "rst $0x%02d", nn);
}

static void jump_a16(MMU& mmu, Z80Registers& regs)
{
    uint16_t addr = mmu.read<uint16_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "jmp $0x%04x", addr);
}

static void ld_mc_a(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "ld (%%c), %%a");
}

static void ld_a_mc(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "ld %%a, (%%c)");
}

static void cb(MMU& mmu, Z80Registers& regs)
{
    uint8_t op = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "cb $0x%02x", op);
}

static void call_a16(MMU& mmu, Z80Registers& regs)
{
    uint16_t addr = mmu.read<uint16_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "call $0x%04x", addr);
}

static void jr(MMU& mmu, Z80Registers& regs)
{
    uint8_t off = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "jr $0x%02x [$%d]", off, (int8_t) off);
}

static void ld_ma16_sp(MMU& mmu, Z80Registers& regs)
{
    uint16_t addr = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "ld ($0x%04x), %%sp", addr);
}

static void ld_mhl_d8(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    uint8_t val = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "ld (%%hl), $0x%02x", val);
}

static void add_hl_sp(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "add %%hl, %%sp");
}

static void add_sp_r8(MMU& mmu, Z80Registers& regs)
{
    uint8_t val = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "add sp, $0x%02x [$%d]", val, (int8_t) val);
}

static void ld_ma16_a(MMU& mmu, Z80Registers& regs)
{
    uint16_t addr = mmu.read<uint16_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "ld ($0x%04x), %%a", addr);
}

static void ld_a_ma16(MMU& mmu, Z80Registers& regs)
{
    uint16_t addr = mmu.read<uint16_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "ld %%a, ($0x%04x)", addr);
}

static void ldh_a_ma8(MMU& mmu, Z80Registers& regs)
{
    uint8_t addr = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "ldh %%a, ($0x%04x)", 0xff00 + addr);
}

static void ldh_ma8_a(MMU& mmu, Z80Registers& regs)
{
    uint8_t addr = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "ldh ($0x%04x), %%a", 0xff00 + addr);
}

static void ld_hl_sppr8(MMU& mmu, Z80Registers& regs)
{
    uint8_t r8 = mmu.read<uint8_t>(regs.PC + 1);
    sprintf(disass_line + HEADER_SIZE, "ld %%hl, (%%sp + $0x%02x) [$%d]", r8,
            (int8_t) r8);
}

static void ld_sp_hl(MMU& mmu, Z80Registers& regs)
{
    (void) mmu;
    (void) regs;
    sprintf(disass_line + HEADER_SIZE, "ld %%sp, %%hl");
}

#define H2(FuncName, OpName, Arg1)                          \
    static void FuncName(MMU& mmu, Z80Registers& regs)      \
    {                                                       \
        OpName(mmu, regs, Arg1);                            \
    }
#define H3(FuncName, OpName, Arg1, Arg2)                    \
    static void FuncName(MMU& mmu, Z80Registers& regs)      \
    {                                                       \
        OpName(mmu, regs, Arg1, Arg2);                      \
    }
#include "disass.gdef"
#undef H2
#undef H3

typedef void (*op)(MMU&, Z80Registers&);

op OPCODES_DISASS[0x100] = {
    // 0x0x
    nop, ld_bc_d16, ld_mbc_a, inc_bc_reg, inc_b_reg, dec_b_reg, ld_b_d8, rlca,
    ld_ma16_sp, add_hl_bc, ld_a_mbc, dec_bc_reg, inc_c_reg, dec_c_reg, ld_c_d8, rrca,
    // 0x1x
    stop, ld_de_d16, ld_mde_a, inc_de_reg, inc_d_reg, dec_d_reg, ld_d_d8, rla,
    jr, add_hl_de, ld_a_mde, dec_de_reg, inc_e_reg, dec_e_reg, ld_e_d8, rra,
    // 0x2x
    jr_nzf_a16, ld_hl_d16, ld_mhlp_a, inc_hl_reg, inc_h_reg, dec_h_reg, ld_h_d8, daa,
    jr_zf_a16, add_hl_hl, ld_a_mhlp, dec_hl_reg, inc_l_reg, dec_l_reg, ld_l_d8, cpl,
    // 0x3x
    jr_ncy_a16, ld_sp_d16, ld_mhlm_a, inc_sp_reg, inc_mhl, dec_mhl, ld_mhl_d8, scf,
    jr_cy_a16, add_hl_sp, ld_a_mhlm, dec_sp_reg, inc_a_reg, dec_a_reg, ld_a_d8, ccf,
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
    cp_a_b, cp_a_c, cp_a_d, cp_a_e, cp_a_h, cp_a_l, cp_a_mhl, cp_a_a,
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

void print_disassembly(MMU& mmu, Z80Registers& regs)
{
#if DISASSEMBLER
    snprintf(disass_line, HEADER_SIZE + 1, "[%04x] Disass : ", regs.PC);
    op tmp = OPCODES_DISASS[mmu.read<uint8_t>(regs.PC)];
    if (tmp == 0) strncat(disass_line, "What?", DISASS_LINE_SIZE);
    else tmp(mmu, regs);
    logging::verbose("%s", disass_line);
#else
    (void) mmu;
    (void) regs;
#endif
}
