from Register import *

MAX_T = 10  # 0-9
MAX_S = 8  # 0-8


class RegisterGroup:
    def __init__(self):  # All Register should be init here.
        self.t = []
        self.s = []
        for i in range(MAX_T):
            self.t.append(Register('t', i))
        for i in range(MAX_S):
            self.s.append(Register('s', i))

    def print_regs(self):
        """
        $s0 = 0 $s1 = 0 $s2 = 0 $s3 = 0
        $s4 = 0 $s5 = 0 $s6 = 0 $s7 = 0
        $t0 = 0 $t1 = 0 $t2 = 0 $t3 = 0
        $t4 = 0 $t5 = 0 $t6 = 0 $t7 = 0
        $t8 = 0 $t9 = 0
        """