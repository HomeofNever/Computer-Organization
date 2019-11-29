from Register import *

MAX_T = 9  # 0-9
MAX_S = 7  # 0-7


class RegisterGroup:
    def __init__(self, forwarding=False):  # All Register should be init here.
        self.t = []
        self.s = []
        for i in range(MAX_T + 1):
            self.t.append(Register('t', i))
        for i in range(MAX_S + 1):
            self.s.append(Register('s', i))

    # Accessors
    def get_s(self, num):
        if self.in_s_range(num):
            return self.s[num]
        else:
            print("ERR: S Register Out of range: {}".format(num))
            return -1

    def get_t(self, num):
        if self.in_t_range(num):
            return self.t[num]
        else:
            print("ERR: T Register Out of range: {}".format(num))
            return -1

    def get(self, prefix, num):
        if prefix == 's':
            return self.get_s(num)
        elif prefix == 't':
            return self.get_t(num)
        else:
            print("ERR: type not recognized: {}".format(prefix))
            return -1

    def get_by_reg(self, reg):
        prefix, num, value = reg.get()
        return self.get(prefix, num)


    def set_s(self, num, val):
        if self.in_s_range(num):
            self.s[num] = val
        else:
            print("ERR: S Register Out of range when set: {}".format(num))

    def set_t(self, num, val):
        if self.in_t_range(num):
            self.t[num] = val
        else:
            print("ERR: T Register Out of range when set: {}".format(num))

    def set(self, prefix, num, val):
        if prefix == 's':
            return self.set_s(num, val)
        elif prefix == 't':
            return self.set_t(num, val)
        else:
            print("ERR: type not recognized: {}".format(prefix))
            return -1

    def set_by_reg(self, reg, val):
        prefix, num, value = reg.get()
        return self.set(prefix, num, val)

    # Helpers
    def in_s_range(self, num):
        return 0 <= num <= MAX_S

    def in_t_range(self, num):
        return 0 <= num <= MAX_T

    # Default
    def __str__(self):
        return """$s0 = {:<14}$s1 = {:<14}$s2 = {:<14}$s3 = {}
$s4 = {:<14}$s5 = {:<14}$s6 = {:<14}$s7 = {}
$t0 = {:<14}$t1 = {:<14}$t2 = {:<14}$t3 = {}
$t4 = {:<14}$t5 = {:<14}$t6 = {:<14}$t7 = {}
$t8 = {:<14}$t9 = {}
        """.format(self.get_s(0).value, self.get_s(1).value, self.get_s(2).value, self.get_s(3).value, self.get_s(4).value,
                   self.get_s(5).value, self.get_s(6).value, self.get_s(7).value, self.get_t(0).value, self.get_t(1).value,
                   self.get_t(2).value, self.get_t(3).value, self.get_t(4).value, self.get_t(5).value, self.get_t(6).value,
                   self.get_t(7).value, self.get_t(8).value, self.get_t(9).value)
