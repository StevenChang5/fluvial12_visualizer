import matplotlib.pyplot as plt

class Hydrograph():
    def __init__(self):
        self.peak = None
        self.end = None
        self.crosssections = {}
    
    def set_peak(self, peak):
        self.peak = peak
    
    def set_end(self, end):
        self.end = end

    def plot_crossection(self, cs_id, time, savePlot=False):
        if time not in self.crosssections[cs_id].coordinates:
            print("Time not found in coordinates")
            return
        x = []
        y = []
        for coordinate in self.crosssections[cs_id].coordinates[time]:
            x.append(coordinate[0])
            y.append(coordinate[1])
        plt.title("Crosssection {} at time {}".format(cs_id, time))
        plt.scatter(x ,y)
        if(savePlot):
            plt.savefig("cs{}t{}.png".format(cs_id,time))
        plt.show()