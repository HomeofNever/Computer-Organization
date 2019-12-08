# Init necessary vars
import sys
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

    stimulator = Stimulator(instructions, MAX_CYCLE, forwarding)

    # Title
    if forwarding:
        print("START OF SIMULATION (forwarding)")
    else:
        print("START OF SIMULATION (no forwarding)")

    for i in range(1, MAX_CYCLE):
        print("----------------------------------------------------------------------------------")
        print("CPU Cycles ===>     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16")

        end = stimulator.run_one_cycle()
        
        print(stimulator)
        # Regs
        print(stimulator.register_group)

        if end:
            break
    
    print("----------------------------------------------------------------------------------")
    print("END OF SIMULATION")
