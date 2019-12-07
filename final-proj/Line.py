from Instruction import *

DEFAULT_STAGE = ["IF", "ID", "EX", "MEM", "WB"]
DEFAULT_PLACEHOLDER = "*"

class Line:
    def __init__(self, instruction=Instruction(), cycle=0, max_cycle=-1):
        self.instruction = instruction
        self.cycle = 0
        self.real_cycles = 0
        self.max_cycle = max_cycle
        self.started_at = cycle
        self.invaild = False
        self.nop1 = 0
        self.nop2 = 0
        self.cycles = ['.' for i in range(self.max_cycle)]
    
    def run_one_stage(self, register_group, last_stage):
        result = None
        if self.cycle < 5:
            if not self.invaild:
                if last_stage > self.cycle:
                    if self.cycle == 1:
                        # Request Vars
                        d1 = self.instruction.d1
                        d2 = self.instruction.d2
                        if not d1.is_integer():
                            d1 = register_group.get_by_reg(d1)
                        if not d2.is_integer():
                            d2 = register_group.get_by_reg(d2)
                        
                        if d1 != None and d2 != None:
                            # No Data Hazard, preceed
                            self.cycles[self.started_at + self.real_cycles] = DEFAULT_STAGE[self.cycle]
                            self.cycle += 1

                    elif self.cycle == 4:
                        r = self.instruction.run(register_group)
                        if self.instruction.o.is_branch():
                            result = r
                        else:
                            register_group.set_by_reg(self.instruction.o, r)
                        self.cycle += 1
                    else:
                        self.cycles[self.started_at + self.real_cycles] = DEFAULT_STAGE[self.cycle]
                        self.cycle += 1
            else:
                self.cycles[self.started_at + self.real_cycles] = "*"
                self.cycle += 1
            
        self.real_cycles += 1

        return result

    def __str__(self):
        nops = ('{:<20}' + '{:<4}' * self.max_cycle).format(*[self.instruction.mips, *self.cycles])
        current_line = ('{:<20}' + '{:<4}' * self.max_cycle).format(*[self.instruction.mips, *self.cycles])

        return ''
