import os
import sys
import matplotlib.pyplot as plt

import utils

def main():
    file = None
    cs_num = None
    time = None
    savePlot = False
    values = []
   
    for i in range(1, len(sys.argv)):
        arg = sys.argv[i]
        if(arg == "-s"):
            savePlot = True
        else:
            values.append(arg)

    if len(values) != 3:
        print("Invalid input, please use format: \n python3 main.py [filename] [cross section number] [time]")

    file = values[0]
    cs_num = int(values[1])
    time = values[2]

    path = os.path.join(os.getcwd(), os.path.join("files", file))
    hydrograph = utils.read_file(path)
    hydrograph.plot_crossection(cs_num,time,savePlot)
    return

if __name__ == "__main__":
    main()