from Instruction import *

MIPS_DEFAULT = "MISSING MIPS"


class Line:
    def __init__(self, line=MIPS_DEFAULT, instruction=Instruction()):
        self.mips = MIPS_DEFAULT
        self.instruction = instruction