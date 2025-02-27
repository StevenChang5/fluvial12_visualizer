import os
import sys

def main():
    path = os.getcwd()
    if(len(sys.argv) < 3):
        print("Invalid input, please use format:\nmain.py [file name] [cross section number]")
        return 0
    file = sys.argv[1]