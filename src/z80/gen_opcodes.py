#! /usr/bin/env python2

import itertools
import os

B_regs = list('ABCDEHL')
W_regs = 'BC,DE,HL,SP'.split(',')
regs = B_regs + W_regs
flags = 'nzf,zf,ncy,cy'.split(',')

f = open(os.path.join(os.path.dirname(__file__), 'genOpcodes.hh'), 'w')

def write_x1(opname, funcname, reg, add_regs=True):
    f.write('X1({opname}, {funcname}, {reg})\n'.format(
        opname = opname.lower(), funcname = funcname,
        reg = ('regs.' if add_regs else '') + str(reg)
    ))

def write_x1_(opname, funcname, reg):
    write_x1(opname, funcname, reg, False)

def write_x2(opname, funcname, reg1, reg2):
    f.write('X2({opname}, {funcname}, {reg1}, {reg2})\n'.format(
        opname = opname.lower(), funcname = funcname, reg1 = reg1, reg2 = reg2
    ))

for func in ['inc_%s_reg', 'dec_%s_reg']:
    for reg in regs:
        write_x1(func % reg, func % ('1B' if len(reg) == 1 else '2B'), reg)

for func in ['ld_%s_mhl', 'ld_mhl_%s']:
    for reg in B_regs:
        write_x1(func % reg, func % '1B_reg', reg)

for func in ['add_hl_%s']:
    for reg in W_regs:
        write_x1(func % reg, func % '2B_reg', reg)

for func in ['ld_%s_%s']:
    for reg in regs:
        size1, size2 = ('1B_reg', 'd8') if len(reg) == 1 else ('2B_reg', 'd16')
        write_x1(func % (reg, size2), func % (size1, size2), reg)
    for reg in itertools.product(B_regs, repeat=2):
        write_x2(func % (reg[0], reg[1]), func % ('1B_reg', '1B_reg'),
                 reg[0], reg[1])

for func in ['add', 'and', 'or', 'xor', 'adc', 'sub', 'sbc', 'cp']:
    for reg in B_regs:
        write_x1('%s_a_%s' % (func, reg), '%s_a_1B_reg' % func, reg)

for func in ['pop', 'push']:
    for reg in W_regs + ['AF']:
        write_x1('%s_%s' % (func, reg), '%s_2B_reg' % func, reg)

for func in ['ret_%s', 'jump_%s_a16', 'call_%s_a16', 'jr_%s']:
    for flag in flags:
        cond = flag.replace('n', '! ').split()
        cond[-1] = '(uint8_t) regs.F.' + cond[-1]
        cond = ' '.join(cond)
        write_x1_(func % flag, func % 'if', cond)

for a in [0, 8, 10, 18, 20, 28, 30, 38]:
    write_x1_('rst_%02dh' % a, 'rst_nn', a)

f.close()
