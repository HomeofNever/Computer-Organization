from Instruction import *

class Line:
    def __init__(self, instruction=Instruction(), cycle=0, max_cycle=-1):
        self.instruction = instruction
        self.cycle = 1
        self.max_cycle = max_cycle
        self.started_at = cycle
        self.cycles = ['.' for i in range(self.max_cycle)]

    def __str__(self):
        return ('{:<20}' + '{:<4}' * self.max_cycle).format(*[self.instruction.mips, *self.cycles])
