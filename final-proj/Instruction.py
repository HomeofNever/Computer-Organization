from Register import *

SUPPORT_INSTRUCTIONS = ['add', 'addi', 'and', 'andi', 'or', 'ori', 'slt', 'slti', 'beq', 'bne']
SUPPORT_INSTS_REGNUM = []
INSTRUCTION_DEFAULT = 'UNKNWON'

class Instruction:
    def __init__(self, inst=INSTRUCTION_DEFAULT, d1=Register(), d2=Register()):
        self.inst = inst
        self.d1 = d1
        self.d2 = d2

    def