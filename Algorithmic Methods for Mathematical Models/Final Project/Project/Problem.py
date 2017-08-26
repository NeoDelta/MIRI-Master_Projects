
import numpy as np
import math

class Type(object):
    def __init__(self, Id):
        self._id = Id
        self._dCity = 0     
        self._cap = 0 
        self._cost = 0

    def addAtr(self, dCity, cap, cost):
        self._dCity = dCity    
        self._cap = cap 
        self._cost = cost

    def getId(self):
        return(self._id)

    def getDist(self):
        return(self._dCity)

    def getCap(self):
        return(self._cap)

    def getCost(self):
        return(self._cost)

class City(object):
    def __init__(self, Id):
        self._id = Id
        self._loc = []     
        self._p = 0 

    def addAtr(self, loc, p):
        self._loc = loc   
        self._p = p

    def getId(self):
        return(self._id)

    def getPoblation(self):
        return(self._p)

    def getLocation(self):
        return(self._loc)

class Location(object):
    def __init__(self, Id):
        self._id = Id
        self._loc = []    

    def addAtr(self, loc):
        self._loc = loc   

    def getId(self):
        return(self._id)

    def getLocation(self):
        return(self._loc)

 

class Problem(object):
    def __init__(self, inputData):
        self.inputData = inputData
        
        nLocations = self.inputData.nLocations
        nCities = self.inputData.nCities
        nTypes = self.inputData.nTypes

        posCities = self.inputData.posCities
        posLocations = self.inputData.posLocations
        p = self.inputData.p

        d_city = self.inputData.d_city
        cap = self.inputData.cap
        cost = self.inputData.cost

        self.d_center = self.inputData.d_center

        self.M = 10000

        self.d_cl = np.zeros((nCities, nLocations))        
        for l in range(nLocations):
            for c in range(nCities):
                x = posCities[c][0] - posLocations[l][0]
                y = posCities[c][1] - posLocations[l][1]
                self.d_cl[c,l] = math.sqrt(pow(x, 2) + pow(y, 2))

        self.d_ll = np.zeros((nLocations, nLocations))
        for l1 in range(nLocations):
            for l2 in range(nLocations):
                x = posLocations[l1][0] - posLocations[l2][0]
                y = posLocations[l1][1] - posLocations[l2][1]
                self.d_ll[l1,l2] = math.sqrt(pow(x, 2) + pow(y, 2))

        self.types = []
        for t in range(nTypes):
            ty = Type(t)
            ty.addAtr(d_city[t], cap[t], cost[t])
            self.types.append(ty)

        self.cities = []
        for c in range(nCities):
            city = City(c)
            city.addAtr(posCities[c], p[c])
            self.cities.append(city)

        self.locations = []
        for l in range(nLocations):
            loc = Location(l)
            loc.addAtr(posLocations[l])
            self.locations.append(loc)
        

    def getCities(self):
        return(self.cities)

    def getTypes(self):
        return(self.types)

    def getLocations(self):
        return(self.locations)

    def checkInstance(self):
        return(True)