'''
AMMM Instance Generator v1.1
Instance Generator class.
Copyright 2016 Luis Velasco and Lluis Gifre.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

import os, random

# Generate instances based on read configuration. 
class InstanceGenerator(object):
    def __init__(self, config):
        self.config = config
    
    def generate(self):
        instancesDirectory = self.config.instancesDirectory
        fileNamePrefix = self.config.fileNamePrefix
        fileNameExtension = self.config.fileNameExtension
        numInstances = self.config.numInstances
        
        numCities = self.config.numCities
        minPoblation = self.config.minPoblation
        maxPoblation = self.config.maxPoblation

        d_center = self.config.d_center
        
        numLocations = self.config.numLocations

        numTypes = self.config.numTypes
        minCapacity = self.config.minCapacity
        maxCapacity = self.config.maxCapacity
        minDistCity = self.config.minDistCity
        maxDistCity = self.config.maxDistCity
        minCost = self.config.minCost
        maxCost = self.config.maxCost

        minX = self.config.minX
        maxX = self.config.maxX
        minY = self.config.minY
        maxY = self.config.maxY


        if(not os.path.isdir(instancesDirectory)):
            raise Exception('Directory(%s) does not exist' % instancesDirectory)

        for i in xrange(0, numInstances):
            instancePath = os.path.join(instancesDirectory, '%s_%d.%s' % (fileNamePrefix, i, fileNameExtension))
            fInstance = open(instancePath, 'w')

            p = []
            for c in xrange(0, numCities):
                p.append(random.randint(minPoblation, maxPoblation))

            cap = []
            cost = []
            d_city = []
            for t in xrange(0, numTypes):
                cap.append(random.randint(minCapacity, maxCapacity))
                cost.append(random.randint(minCost, maxCost))
                d_city.append(random.randint(minDistCity, maxDistCity))
            
            
            fInstance.write('nCities=%d;\n' % numCities)
            fInstance.write('nLocations=%d;\n' % numLocations)
            fInstance.write('nTypes=%d;\n' % numTypes)

            fInstance.write('d_center=%d;\n' % d_center)
            
            # translate vector of floats into vector of strings and concatenate that strings separating them by a single space character
            fInstance.write('p=[%s];\n' % (' '.join(map(str, p))))
            fInstance.write('cap=[%s];\n' % (' '.join(map(str, cap))))
            fInstance.write('d_city=[%s];\n' % (' '.join(map(str, d_city))))
            fInstance.write('cost=[%s];\n' % (' '.join(map(str, cost))))
            
            fInstance.write('posCities=[\n')
            for c in xrange(0, numCities):
                posCity = [0]*2 # create a vector of 0's
                posCity[0] = random.randint(minX, maxX)
                posCity[1] = random.randint(minY, maxY)
                
                # translate vector of integers into vector of strings and concatenate that strings separating them by a single space character
                fInstance.write('\t[%s]' % (' '.join(map(str, posCity))))
            fInstance.write('];\n')

            fInstance.write('posLocations=[\n')
            for c in xrange(0, numLocations):
                posLoc = [0]*2 # create a vector of 0's
                posLoc[0] = random.randint(minX, maxX)
                posLoc[1] = random.randint(minY, maxY)
                
                # translate vector of integers into vector of strings and concatenate that strings separating them by a single space character
                fInstance.write('\t[%s]' % (' '.join(map(str, posLoc))))
            fInstance.write('];\n')

            fInstance.close()
