class Crosssection():
    def __init__(self, id, name, num):
        self.id = id
        self.name = name
        self.num_coordinates = int(num)
        self.coordinates = {}
    def add_coordinates(self, station, elevation, time):
        if time not in self.coordinates:
            self.coordinates[time] = []
            self.coordinates[time].append([float(station),float(elevation)])
        else:
            self.coordinates[time].append([float(station),float(elevation)])

    def get_name(self):
        return self.name