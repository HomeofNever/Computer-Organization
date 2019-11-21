# Init necessary vars
import sys
from RegisterGroup import *
from Line import *

SPLIT_INST = " "
SPLIT_REG = ","
MAX_CYCLE = 16

BRANCH_PREDICTION_TAKEN = False

if __name__ == '__main__':
    # Start here
    mode = sys.argv[1]
    forwarding = mode == 'F'  # True for forwarding, False for no forwarding
    filename = sys.argv[2]

    file = open(filename, "r")
    """
    ori $s1,$zero,451
    Parsing
    """

    lines = []
    for line in file:
        line = line.strip()
        if line:
            if line.endswith(":"):
                # This is a branch
                branch = Register(prefix=PREFIX_BRANCH, value=line[:-1])
                lines.append(Line(line, Instruction(inst=PREFIX_BRANCH, o=branch), MAX_CYCLE))
            else:
                space = line.split(SPLIT_INST)
                inst = space[0]
                regs = space[1].split(SPLIT_REG)
                lines.append(Line(line,
                                  Instruction(inst,
                                              parse_register(regs[0]),
                                              parse_register(regs[1]),
                                              parse_register(regs[2])),
                                  MAX_CYCLE))

    # Init Instance
    register_group = RegisterGroup()

    # Title
    if forwarding:
        print("START OF SIMULATION (forwarding)")
    else:
        print("START OF SIMULATION (no forwarding)")

    '''
    More specifically, the first column must have a width of exactly 20 characters, while each subsequent
    column (corresponding to each clock cycle) must have a width of exactly four characters. Include
    a space between each column and left-justify each column. Further, be sure there are no trailing
    spaces on the end of each line of output.
    Finally, show no more than 16 cycles in your simulation, meaning that if you reach cycle 16, display
    that last cycle and end your simulation.
    '''
    for i in range(1, MAX_CYCLE):
        print("----------------------------------------------------------------------------------")
        print("CPU Cycles ===>     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16")

        # Insts
        for j in lines:
            print(j)

        # Regs
        print(register_group)

        # Identity number element we need, from instruction
