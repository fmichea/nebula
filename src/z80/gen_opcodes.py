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
        'h3': 'H3(%s, %s, %s, %s)\n',
        'h3ro': 'H3RO(%s, %s, %s, %s)\n',
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
    std.write('x0', 'rst_%02dh' % a, 'rst_nn', '0x%d' % a)

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
        hname, delay = 'h3', 8
        if func.startswith('bit'):
            hname, delay = 'h3ro', 4
        cb.write(hname, func % (bit, 'mhl'), func % ('nb', '1B_reg'), bit, delay)

# Disassembly
dis = DefMacro('disass.def')

for op in ['inc', 'dec']:
    for reg in ALL_regs:
        reg = reg.lower()
        dis.write('h2', '%s_%s_reg' % (op, reg), '%s_reg' % op, '"%%%s"' % reg)

for op in ['push', 'pop']:
    for reg in list(W_regs[:-1] + ['af']):
        reg = reg.lower()
        dis.write('h2', '%s_%s' % (op, reg), '%s_reg' % op, '"%%%s"' % reg)

for op in ['ld_a_m%s', 'ld_m%s_a']:
    for reg in ['bc', 'de', 'hl+', 'hl-']:
        regn = reg.replace('+', 'p').replace('-', 'm')
        dis.write('h2', op % regn, op % 'reg', '"%%%s"' % reg)

tmp = map(lambda a: ('8', a.lower()), B_regs)
tmp += map(lambda a: ('16', a.lower()), W_regs)
for (size, reg) in tmp:
    dis.write('h2', 'ld_%s_d%s' % (reg, size), 'ld_reg_val<uint%s_t>' % size,
              '"%%%s"' % reg)

for op in ['add', 'sub', 'and', 'or', 'adc', 'sbc', 'xor', 'cp']:
    dis.write('h2', '%s_a_d8' % op, 'op_a_d8', '"%s"' % op)
    dis.write('h3', '%s_a_mhl' % op, 'op_a_reg', '"%s"' % op, '"(%hl)"')

    for reg in B_regs:
        reg = reg.lower()
        dis.write('h3', '%s_a_%s' % (op, reg), 'op_a_reg', '"%s"' % op,
                  '"%%%s"' % reg)

for op in ['inc', 'dec', 'jump']:
    dis.write('h2', '%s_mhl' % op, 'op_mhl', '"%s"' % op)

for op in ['nop', 'stop', 'rlca', 'rla', 'daa', 'scf', 'rrca', 'rra', 'cpl',
           'ccf', 'ei', 'di', 'halt', 'ret', 'reti']:
    dis.write('h2', op, 'uniq_operation', '"%s"' % op)

for reg in B_regs:
    reg = reg.lower()
    dis.write('h3', 'ld_%s_mhl' % reg, 'ld_reg_reg', '"%s"' % reg, '"(hl)"')
    dis.write('h3', 'ld_mhl_%s' % reg, 'ld_reg_reg', '"(hl)"', '"%s"' % reg)

for flag in ['zf', 'nzf', 'cy', 'ncy']:
    for op in ['call', 'jump', 'jr']:
        dis.write('h2', '%s_%s_a16' % (op, flag), '%s_if_a16' % op, '"%s"' % flag)
    dis.write('h2', 'ret_%s' % flag, 'ret_if', '"%s"' % flag)

for a in [0, 8, 10, 18, 20, 28, 30, 38]:
    dis.write('h2', 'rst_%02dh' % a, 'rst_nn', a)

for a in ['bc', 'de', 'hl']:
    dis.write('h2', 'add_hl_%s' % a, 'add_hl_reg', '"%%%s"' % reg)

for (r1, r2) in itertools.product(B_regs, B_regs):
    r1, r2 = r1.lower(), r2.lower()
    dis.write('h3', 'ld_%s_%s' % (r1, r2), 'ld_reg_reg',
              '"%%%s"' % r1, '"%%%s"' % r2)
