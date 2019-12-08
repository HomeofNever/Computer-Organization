from Instruction import *
from Branches import *

class Instructions:
    def __init__(self, file):
        self.instructions = []
        self.branches = Branches()
        self.parse(file)

    def parse(self, file):
        i = 0
        for line in file:
            line = line.strip()
            if line:
                if line.endswith(":"):
                    # This is a branch
                    self.branches.set(line[:-1], i + 1)
                else:
                    space = line.split(SPLIT_INST)
                    inst = space[0]
                    regs = space[1].split(SPLIT_REG)
                    self.instructions.append(Instruction(line, inst,
                                                parse_register(regs[0]),
                                                parse_register(regs[1]),
                                                parse_register(regs[2])))
                    i += 1

    def get_instruction_by_index(self, index):
        if index < len(self.instructions):
            return self.instructions[index]
        else:
            return None

    def get_instruction_by_key(self, key):
        br = self.branches.get(key)
        if br:
            return self.get_instruction_by_index(br)
        else:
            print("ERR: branch NOT Found")