from Line import *

class Stimulator:
    def __init__(self, instructions, register_group, max_cycle = -1, forwarding = False):
        self.instructions = instructions
        self.register_group = register_group
        self.max_cycle = max_cycle
        self.cycle = 0
        self.forwarding = forwarding
        self.runtime = []
        self.program_counter = 0

    
    def run_one_cycle(self):
        # First, we need to insert one more from the program counter.
        inst = self.instructions.get_instruction_by_index(
                self.program_counter
        )
        if inst is not None:
            self.runtime.append(Line(inst, self.cycle, self.max_cycle))
        
        self.program_counter += 1

        for i in self.runtime:
            i.run_one_stage()
        
        self.cycle += 1

    def is_control_hazard(self):
        return False

    def __str__(self):
        s = ''
        for i in self.runtime:
            s += str(i) + '\n'
