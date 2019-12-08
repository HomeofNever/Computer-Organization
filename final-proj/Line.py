from Instruction import *

DEFAULT_STAGE = ["IF", "ID", "EX", "MEM", "WB"]
DEFAULT_PLACEHOLDER = "*"
DEFAULT_CYCLES = lambda cycle : ['.' for i in range(cycle)]

class Line:
    def __init__(self, instruction=Instruction(), cycle=0, max_cycle=-1):
        self.instruction = instruction
        self.cycle = 0
        self.max_cycle = max_cycle
        self.started_at = cycle
        self.invaild = False
        self.nop = 0
        self.d1 = None
        self.d2 = None
        self.nops = []
        self.cycles = DEFAULT_CYCLES(max_cycle)
    
    def run_one_stage(self, register_group, last_cycle, current_cycle):
        result = None
        if self.cycle < 5:
            if not self.invaild:
                if last_cycle > self.cycle:
                    if self.cycle == 0:
                        # Lock var in if stage
                        o = self.instruction.o
                        if not o.is_branch():
                            register_group.lock_by_reg(o)
                        self.cycles[current_cycle] = DEFAULT_STAGE[self.cycle]
                        self.cycle += 1
                    elif self.cycle == 1:
                        # Request Vars
                        self.d1 = self.instruction.d1
                        self.d2 = self.instruction.d2
                        if not d1.is_integer():
                            self.d1 = register_group.get_by_reg(d1)
                        else:
                            self.d1 = self.d1.get_value()
                        if not d2.is_integer():
                            self.d2 = register_group.get_by_reg(d2)
                        else:
                            self.d2 = self.d2.get_value()
                        
                        if self.d1 != None and self.d2 != None:
                            # No Data Hazard, preceed
                            self.cycles[current_cycle] = DEFAULT_STAGE[self.cycle]
                            self.cycle += 1
                        else:
                            # Data Hazard: cannot access reg
                            self.cycles[current_cycle] = DEFAULT_STAGE[self.cycle]
                            self.nop += 1
                            self.nops.append(self.cycles.copy())
                    elif self.cycle == 2:
                        # EX stage
                        r = self.instruction.run(self.d1, self.d2)
                        if not self.instruction.o.is_branch():
                            # Set FW Reg
                            register_group.set_by_reg(self.instruction.o, r, True)
                        self.cycles[current_cycle] = DEFAULT_STAGE[self.cycle]
                        self.cycle += 1
                    elif self.cycle == 4:
                        # Write Back
                        r = self.instruction.run(self.d1, self.d2)
                        if self.instruction.o.is_branch():
                            result = r
                        else:
                            # When Set, Unlock Reg
                            register_group.set_by_reg(self.instruction.o, r)
                            register_group.unlock_by_reg(self.instruction.o)
                        
                        self.cycles[current_cycle] = DEFAULT_STAGE[self.cycle]
                        self.cycle += 1
                    else:
                        # Normal Stage
                        self.cycles[current_cycle] = DEFAULT_STAGE[self.cycle]
                        self.cycle += 1
                else:
                    # Current Stage have to stall for last one to finish
                    self.cycles[current_cycle] = DEFAULT_STAGE[self.cycle - 1]
            else:
                self.cycles[current_cycle] = "*"
                self.cycle += 1

        self.update_nops(current_cycle)
        return result

    def update_nops(self, current_cycle):
        for i in range(self.nop):
            self.nops[i][current_cycle] = "*"

    def get_cycle(self):
        return self.cycle

    def set_invalid(self):
        self.invaild = True

    def __str__(self):
        nops = ''
        for i in self.nops:
            nops += ('{:<20}' + '{:<4}' * self.max_cycle).format(*["nop", *i]) + "\n"
        
        current_line = ('{:<20}' + '{:<4}' * self.max_cycle).format(*[self.instruction.mips, *self.cycles])

        return nops + current_line