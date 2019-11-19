# Init necessary vars
import sys

SPLIT_INST = " "
SPLIT_REG = ","

if __name__ == 'main':
    # Start here
    mode = sys.argv[1]
    filename = sys.argv[2]

    file = open(filename, "r")
    """
    ori $s1,$zero,451
    Parsing
    """

    for line in file:
        space = line.split(SPLIT_INST)[0]
        inst = space[0]
        regs = space[1].split(SPLIT_REG)

        # Identity number element we need, from instruction
