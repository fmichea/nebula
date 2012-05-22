#! /usr/bin/env python2

import itertools
import os

B_regs = list('ABCDEHL')
W_regs = 'BC,DE,HL,SP'.split(',')
ALL_regs = B_regs + W_regs
flags = 'nzf,zf,ncy,cy'.split(',')

class DefMacro(object):
    MACROS = {
        'x0': 'X1(%s, %s, %s)\n',
        'x1': 'X1(%s, %s, regs.%s)\n',
        'x2': 'X2(%s, %s, regs.%s, regs.%s)\n',
        'x3': 'X2(%s, %s, regs.%s, %s)\n',
        'h1': 'H1(%s, %s)\n',
        'h2': 'H2(%s, %s, %s)\n',
    }

    def __init__(self, name):
        self.f = open(os.path.join(os.path.dirname(__file__), name), 'w')

    def __del__(self):
        self.f.close()

    def write(self, macro, *args):
        self.f.write(self.MACROS[macro] % args)

# Standart opcodes.
std = DefMacro('opcodes.def')

for func in ['inc_%s_reg', 'dec_%s_reg']:
    for reg in ALL_regs:
        std.write('x1', func % reg.lower(), func % ('1B' if len(reg) == 1 else '2B'), reg)

for func in ['ld_%s_mhl', 'ld_mhl_%s']:
    for reg in B_regs:
        std.write('x1', func % reg.lower(), func % '1B_reg', reg)

for func in ['add_hl_%s']:
    for reg in W_regs:
        std.write('x1', func % reg.lower(), func % '2B_reg', reg)

for func in ['ld_%s_%s']:
    for reg in ALL_regs:
        size1, size2 = ('1B_reg', 'd8') if len(reg) == 1 else ('2B_reg', 'd16')
        std.write('x1', func % (reg.lower(), size2), func % (size1, size2), reg)
    for reg in itertools.product(B_regs, repeat=2):
        std.write('x2', func % (reg[0].lower(), reg[1].lower()),
                  func % ('1B_reg', '1B_reg'), reg[0], reg[1])

for func in ['add', 'and', 'or', 'xor', 'adc', 'sub', 'sbc', 'cp']:
    for reg in B_regs:
        std.write('x1', '%s_a_%s' % (func, reg.lower()), '%s_a_1B_reg' % func, reg)

for func in ['pop', 'push']:
    for reg in W_regs + ['AF']:
        std.write('x1', '%s_%s' % (func, reg.lower()), '%s_2B_reg' % func, reg)

for func in ['ret_%s', 'jump_%s_a16', 'call_%s_a16', 'jr_%s']:
    for flag in flags:
        cond = flag.replace('n', '! ').split()
        cond[-1] = 'regs.F.' + cond[-1] + '.get()'
        cond = ' '.join(cond)
        std.write('x0', func % flag, func % 'if', cond)

for a in [0, 8, 10, 18, 20, 28, 30, 38]:
    std.write('x0', 'rst_%02dh' % a, 'rst_nn', a)

# CB Opcodes
cb = DefMacro('cbopcodes.def')

funcs = ['rlc_%s', 'rl_%s', 'sla_%s', 'rrc_%s', 'rr_%s', 'sra_%s', 'srl_%s',
         'swap_%s']
for func in funcs:
    for reg in B_regs:
        cb.write('x1', func % reg.lower(), func % '1B_reg', reg)
    cb.write('h1', func % 'mhl', func % '1B_reg')

for func in ['bit_%s_%s', 'set_%s_%s', 'res_%s_%s']:
    for reg in B_regs:
        for bit in xrange(8):
            cb.write('x3', func % (bit, reg.lower()), func % ('nb', '1B_reg'), reg, bit)
    for bit in xrange(8):
        cb.write('h2', func % (bit, 'mhl'), func % ('nb', '1B_reg'), bit)
