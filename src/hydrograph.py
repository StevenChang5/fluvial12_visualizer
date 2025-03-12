import matplotlib.pyplot as plt

class Hydrograph():
    def __init__(self):
        self.peak = None
        self.end = None
        self.crosssections = {}
        self.sed_yield_tons = {}
    
    def set_peak(self, peak):
        self.peak = float(peak)
        self.peak += 0.01
        self.peak = "{}".format(self.peak)
    
    def set_end(self, end):
        self.end = float(end)
        self.end += 0.01
        self.end = "{}".format(self.peak)

    def add_SYD(self, section:float, syd:float, time:str):
        if time not in self.sed_yield_tons:
            self.sed_yield_tons[time] = []
            self.sed_yield_tons[time].append([section,syd])
        else:
            self.sed_yield_tons[time].append([section,syd])

    def plot_crossection(self, cs_id, time, save_plot=False):
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
        if(save_plot):
            plt.savefig("cs{}t{}.png".format(cs_id,time))
        plt.show()
    
    def plot_crosssection(self, cs_id, save_plot=False):
        plt.title("Crosssection {}".format(cs_id))

        x = []
        y = []
        for coordinate in self.crosssections[cs_id].coordinates['0']:
            x.append(coordinate[0])
            y.append(coordinate[1])
        plt.plot(x,y,'--', color='red', label="Initial",linewidth='2',marker='.')

        a = []
        b = []
        for coordinate in self.crosssections[cs_id].coordinates[self.peak]:
            a.append(coordinate[0])
            b.append(coordinate[1])
        plt.plot(a,b,'--', color='limegreen',label="Peak",linewidth = '1.5',marker='.')

        c = []
        d = []
        for coordinate in self.crosssections[cs_id].coordinates[self.end]:
            c.append(coordinate[0])
            d.append(coordinate[1])
        plt.plot(c,d,'--', color='blue', label="End",linewidth='1',marker='.')

        plt.xlabel("Station")
        plt.ylabel("Elevation")
        plt.legend()
        if(save_plot):
            plt.savefig("cs{}.png".format(cs_id))
        plt.show()
    
    def plot_SYD(self, save_plot=False):
        plt.title("Sediment Yield Tons")
        
        x = []
        y = []
        for section in self.sed_yield_tons[self.peak]:
            x.append(section[0])
            y.append(section[1])
        plt.plot(x,y,'--', color='red', label="Peak",linewidth='2',marker='.')

        a = []
        b = []
        for section in self.sed_yield_tons[self.end]:
            a.append(section[0])
            b.append(section[1])
        plt.plot(a,b,'--', color='limegreen',label="End",linewidth = '1.5',marker='.')

        plt.xlabel("Section")
        plt.ylabel("Sed. Yield Tons")
        plt.legend()
        if(save_plot):
            plt.savefig("syd.png")
        plt.show()