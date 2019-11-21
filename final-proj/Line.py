from Instruction import *

MIPS_DEFAULT = "MISSING MIPS"


class Line:
    def __init__(self, line=MIPS_DEFAULT, instruction=Instruction(), max_cycle=-1):
        self.mips = line
        self.instruction = instruction
        self.is_branch = False
        self.cycle = 1
        self.max_cycle = max_cycle
        self.cycles = ['.' for i in range(self.max_cycle)]

    def __str__(self):
        return '{:<20}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}{:<4}'.format(*[self.mips, *self.cycles])
