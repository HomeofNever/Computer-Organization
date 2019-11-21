from Register import *

NORMAL_INSTRUCTIONS = ['add', 'addi', 'and', 'andi', 'or', 'ori', 'slt', 'slti']
BRANCH_INSTRUCTIONS = ['beq', 'bne']
SUPPORT_INSTRUCTIONS = [*NORMAL_INSTRUCTIONS, *BRANCH_INSTRUCTIONS]
INSTRUCTION_DEFAULT = 'UNKNWON'


class Instruction:
    def __init__(self, inst=INSTRUCTION_DEFAULT, d1=Register(), d2=Register(), d3=Register()):
        self.inst = inst
        if inst in BRANCH_INSTRUCTIONS:
            self.d1 = d1
            self.d2 = d2
            self.o = d3
        else:
            self.d1 = d2
            self.d2 = d3
            self.o = d1

    def is_branch(self):
        return self.inst == PREFIX_BRANCH
