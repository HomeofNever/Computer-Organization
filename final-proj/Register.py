REGISTER_PREFIX = ['s', 't', 'v', 'a']
PREFIX_BRANCH = 'branch'
PREFIX_ZERO = 'zero'
PREFIX_INTEGER = 'integer'
CONST_PREFIX = [PREFIX_BRANCH, PREFIX_ZERO, PREFIX_INTEGER]
PREFIX_DEFAULT = 'n'
SYB_DOLLAR = '$'
NUM_DEFAULT = -1
VALUE_DEFAULT = 0


def parse_register(reg):
    if reg[0] == SYB_DOLLAR:
        reg = reg[1:]
    if reg == PREFIX_ZERO:
        return Register(PREFIX_ZERO)
    elif reg[0].isdigit():
        return Register(PREFIX_INTEGER, value=int(reg))
    elif reg[1].isdigit() and (reg[0] in REGISTER_PREFIX):
        return Register(reg[0], int(reg[1]))
    elif reg[0].isalpha():
        return Register(PREFIX_BRANCH, value=reg)
    else:
        print("ERR: Recongized Register Type: {}".format(reg))
        return None


class Register:
    def __init__(self, prefix=PREFIX_DEFAULT, num=NUM_DEFAULT, value=VALUE_DEFAULT, ready=False):
        self.prefix = prefix
        self.num = num
        self.value = value
        self.ready = ready

    def get(self):
        return self.prefix, self.num, self.value

    def get_value(self):
        return self.value

    def set(self, value):
        self.value = value

    def is_empty(self):
        return self.prefix == PREFIX_DEFAULT and self.num == NUM_DEFAULT

    def is_register(self):
        return self.prefix not in CONST_PREFIX

    def is_branch(self):
        return self.prefix == PREFIX_BRANCH

    def is_integer(self):
        return self.prefix == PREFIX_INTEGER or self.prefix == PREFIX_ZERO

    def is_ready(self):
        return self.ready
    
    def set_ready(self, ready = True):
        self.ready = ready

    def reset(self):
        self.value = VALUE_DEFAULT
        self.ready = False

    def __eq__(self, other):
        if other == None:
            return False
        else:
            return self.prefix == other.prefix and self.num == other.num

    def __str__(self):
        return '${}{} = {}'.format(self.prefix, self.num, self.value)
