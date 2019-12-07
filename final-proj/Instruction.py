from Register import *
import copy

NORMAL_INSTRUCTIONS = ['add', 'addi', 'and', 'andi', 'or', 'ori', 'slt', 'slti']
BRANCH_INSTRUCTIONS = ['beq', 'bne']
SUPPORT_INSTRUCTIONS = [*NORMAL_INSTRUCTIONS, *BRANCH_INSTRUCTIONS]
INSTRUCTION_DEFAULT = 'UNKNWON'
SPLIT_INST = " "
SPLIT_REG = ","
MIPS_DEFAULT = "MISSING MIPS"

class Instruction:
    def __init__(self, mips=MIPS_DEFAULT, inst=INSTRUCTION_DEFAULT, d1=Register(), d2=Register(), d3=Register()):
        self.mips = mips
        self.inst = inst
        if inst in BRANCH_INSTRUCTIONS:
            self.d1 = d1
            self.d2 = d2
            self.o = d3
        else:
            self.d1 = d2
            self.d2 = d3
            self.o = d1

    def run(self, register_group):
        # Run Command Immediately and get result
        switcher = {
            'add': lambda a, b: a + b,
            'addi': lambda a, b: a + b,
            'and': lambda a, b: a & b,
            'or': lambda a, b: a | b,
            'ori': lambda a, b: a | b,
            'slt': lambda a, b: 0 if a < b else 1,
            'slti': lambda a, b: 0 if a < b else 1,
            'beq': lambda a, b: True if a == b else False,
            'bne': lambda a, b: True if a != b else False,
        }

        func = switcher.get(self.inst, lambda : print("ERR: Inst {} Not Found".format(self.inst)))

        d1 = self.d1
        if not d1.is_integer():
            d1 = register_group.get_by_reg(d1)
        d2 = self.d2
        if not d2.is_integer():
            d2 = register_group.get_by_reg(d2)

        return func(d1, d2)

    def copy(self):
        return copy.deepcopy(self)

