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

    if (len(values) != 3) and (len(values) != 2):
        print("Invalid input, please use format: \n python3 main.py [filename] [cross section number] [time]")

    if len(values) == 3:
        # Given time paramter, plot that time only
        file = values[0]
        cs_num = int(values[1])
        time = values[2]
    elif len(values) == 2:
        # Given no time parameter, plot @ beginning, peak, and end
        file = values[0]
        cs_num = int(values[1])

    path = os.path.join(os.getcwd(), os.path.join("files", file))
    hydrograph = utils.read_file(path)
    if(len(values) == 3):
        hydrograph.plot_crossection(cs_num,time,savePlot)
    elif(len(values) == 2):
        hydrograph.plot_crosssection(cs_num,savePlot)
    return

if __name__ == "__main__":
    main()