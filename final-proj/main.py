# Init necessary vars
import sys
from RegisterGroup import *
from Instructions import *
from Stimulator import *

MAX_CYCLE = 16

BRANCH_PREDICTION_TAKEN = False

if __name__ == '__main__':
    # Start here
    mode = sys.argv[1]
    forwarding = mode == 'F'  # True for forwarding, False for no forwarding
    filename = sys.argv[2]

    file = open(filename, "r")

    instructions = Instructions(file)
    # Init Instance
    register_group = RegisterGroup()

    stimulator = Stimulator(instructions, register_group, MAX_CYCLE, forwarding)

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

        stimulator.run_one_cycle()
        
        print(stimulator)
        # Regs
        print(register_group)

        # Identity number element we need, from instruction
