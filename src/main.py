import os
import sys
import matplotlib.pyplot as plt

import utils

def main():
    if(len(sys.argv) < 3):
        print("Invalid input, please use format:\nmain.py [file name] [cross section number]")
        return 0
    file = sys.argv[1]
    cs_num = int(sys.argv[2])
    time = sys.argv[3]
    print(file, cs_num, time)
    path = os.path.join(os.getcwd(), os.path.join("files", file))
    hg = utils.read_file(path)
    print(len(hg.crosssections[1].coordinates['0']), hg.crosssections[1].num_coordinates)

    hg.plot_crossection(cs_num,time)
    return

if __name__ == "__main__":
    main()