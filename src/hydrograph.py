import matplotlib.pyplot as plt

class Hydrograph():
    def __init__(self):
        self.peak = None
        self.end = None
        self.crosssections = {}
    
    def set_peak(self, peak):
        self.peak = float(peak)
        self.peak += 0.01
        self.peak = "{}".format(self.peak)
    
    def set_end(self, end):
        self.end = float(end)
        self.end += 0.01
        self.end = "{}".format(self.peak)

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
        plt.plot(x ,y, 'ro-')
        if(savePlot):
            plt.savefig("cs{}t{}.png".format(cs_id,time))
        plt.show()
    
    def plot_crosssection(self, cs_id, savePlot=False):
        plt.title("Crosssection {}".format(cs_id))

        x = []
        y = []
        for coordinate in self.crosssections[cs_id].coordinates['0']:
            x.append(coordinate[0])
            y.append(coordinate[1])
        plt.plot(x,y,'r.-',label="Initial")

        a = []
        b = []
        for coordinate in self.crosssections[cs_id].coordinates[self.peak]:
            a.append(coordinate[0])
            b.append(coordinate[1])
        plt.plot(a,b,'g.-',label="Peak")

        c = []
        d = []
        for coordinate in self.crosssections[cs_id].coordinates[self.end]:
            c.append(coordinate[0])
            d.append(coordinate[1])
        plt.plot(c,d,'b.-',label="End")

        plt.xlabel("Station")
        plt.ylabel("Elevation")
        plt.legend()
        if(savePlot):
            plt.savefig("cs{}.png".format(cs_id))
        plt.show()