from crosssection import Crosssection
from hydrograph import Hydrograph
import math

def read_file(path)->Hydrograph:
    file = open(path)
    id = 1
    hg = Hydrograph()
    lines = file.readlines()
    for row in range(len(lines)):
        line = lines[row].split()
        if(len(line) == 0): continue
        if line[0] == 'G1':
            # Get peak and end values from G1 header
            hg.set_peak(line[8])
            hg.set_end(line[9])
        elif line[0] == 'X1':
            # Collect crosssection metadata at initial time
            cs = Crosssection(id, line[1], line[2].split('.')[0])
            subrow = row+1
            while(lines[subrow].split()[0] != 'GR'):
                subrow += 1
            for _ in range(math.ceil(cs.num_coordinates/5)):
                subline = lines[subrow].split()
                for j in range(1, len(subline), 2):
                    cs.add_coordinates(subline[j+1],subline[j],'0')
                subrow += 1
            hg.crosssections[id] = cs
            id += 1
        elif line[0] == 'ID':
            # Extract information from header
            subrow = row+1
            subline = lines[subrow].split()
            cs_id = int(subline[0])
            time = subline[5]
            cs = hg.crosssections[cs_id]
            # Add coordinates @ time to crosssection object
            subrow += 4
            for _ in range(math.ceil(cs.num_coordinates/3)):
                subline = lines[subrow].split()
                for j in range(0, len(subline),4):
                    cs.add_coordinates(subline[j+3], subline[j], time)
                subrow += 1
        elif line[0] == 'TIME':
            time = line[2]
            subrow = row+5
            for id in range(1,len(hg.crosssections)+1):
                subline = lines[subrow].split()
                syd = exponent_to_float(subline[10])
                hg.add_SYD(float(subline[0]), syd, time)
                subrow += 1


    return hg

def exponent_to_float(exponent: str)->float:
    num = exponent.split('E')
    pos = True
    if len(num) == 1:
        return float(num)
    else:
        value = float(num[0])
        exponent = num[1]
        if exponent[0] == '-':
            pos = False
        exp_val = float(exponent[1:])
        if(pos):
            return float(value * (10 ** exp_val))
        else:
            return float(value * (10 ** -exp_val))