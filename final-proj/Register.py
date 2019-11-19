REGISTER_PREFIX = ['s', 't', 'v', 'a', 'zero']
PREFIX_DEFAULT = 'n'
NUM_DEFAULT = -1
VALUE_DEFAULT = 0


class Register:
    def __init__(self, prefix=PREFIX_DEFAULT, num=NUM_DEFAULT, value=VALUE_DEFAULT):
        self.prefix = prefix
        self.num = num
        self.value = value

    def get(self):
        return self.prefix, self.num, self.value

    def set(self, value):
        self.value = value

    def is_empty(self):
        return self.prefix == PREFIX_DEFAULT and self.num == NUM_DEFAULT

    def __eq__(self, other):
        return self.prefix == other.prefix and self.num == other.num

    def __str__(self):
        return '${}{} = {}'.format(self.prefix, self.num, self.value)
