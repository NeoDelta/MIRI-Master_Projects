import copy, time
from Problem import Problem

# Assignment class stores the load of the highest loaded CPU
# when a task is assigned to a CPU.  
class Assignment(object):
    def __init__(self, cityId, primId, secId, cost):
        self.cityId = cityId
        self.primId = primId
        self.secId = secId
        self.cost = cost

# Solution includes functions to manage the solution, to perform feasibility
# checks and to dump the solution into a string or file.
class Solution(Problem):
    @staticmethod
    def createEmptySolution(config, problem):
        solution = Solution(problem.inputData)
        solution.setVerbose(config.verbose)
        return(solution)

    def __init__(self, inputData):
        super(Solution, self).__init__(inputData)
        
        self.cityToPrimary = {}           # hash table: City => Primary center location id
        self.cityToSecondary = {}         # hash table: City => Secondary center location id
        self.locationToType = {}          # hash table: Location => Center type
        
        # vector of available capacities per CPU initialized as a copy of maxCapacityPerCPUId vector.
        # self.availCapacityPerCPUId = copy.deepcopy(self.maxCapacityPerCPUId)
                
        self.cost = 0.0     
        
        self.feasible = True
        self.verbose = False
    
    def setVerbose(self, verbose):
        if(not isinstance(verbose, (bool)) or (verbose not in [True, False])):
            raise Exception('verbose(%s) has to be a boolean value.' % str(verbose))
        self.verbose = verbose
    
    def makeInfeasible(self):
        self.feasible = False
        self.cost = 20000
    
    def isFeasible(self):
        return(self.feasible)
    
    def updateCost(self):
        self.cost = 0.0
        for loc in self.locationToType.keys():
            typeId = self.locationToType[loc]
            self.cost += self.types[typeId].getCost()
            
    def isFeasibleToAssignCityToLocAsPrim(self, cityId, primId):
        if(self.cityToPrimary.has_key(cityId)):
            if(self.verbose): print('City(%s) already has a primary center assigned.' % str(cityId))
            return(False)
        
        city = self.cities[cityId]
        prim = self.locations[primId]
        dist = self.d_cl[cityId,primId]
        poblation = city.getPoblation()

        if(self.locationToType.has_key(primId)):
            cTypeId = self.locationToType[primId]
            cType = self.types[cTypeId]
            cCap = cType.getCap()

            cPrimServed = 0
            for k in self.cityToPrimary.keys():
                if self.cityToPrimary[k] == primId:
                    cPrimServed += self.cities[k].getPoblation()

            cSecServed = 0
            for k in self.cityToSecondary.keys():
                if self.cityToSecondary[k] == primId:
                    cSecServed += self.cities[k].getPoblation()
            

            totalPob = cPrimServed + cSecServed/10 + poblation

            if(cCap < totalPob or dist > cType.getDist()):
                selectedTypeId = self.types[0].getId()
                isPossible = False
                for t in self.types:
                    if(t.getDist() >= dist and t.getCap() >= totalPob):

                        isTpossible = True

                        for k in self.cityToPrimary.keys():
                            if self.d_cl[k,primId] > t.getDist(): 
                                isTpossible = False

                        for k in self.cityToSecondary.keys():
                            if self.d_cl[k,primId] > t.getDist():
                                isTpossible = False

                        if(isTpossible):
                            if(self.types[selectedTypeId].getCost()>t.getCost()):
                                isPossible = True
                                selectedTypeId = t.getId()

                    else: isPossible = False

                if(isPossible):
                    self.cityToPrimary.update({cityId: primId})
                    self.locationToType[primId] = selectedTypeId
                    return(True)

                return(False)

            self.cityToPrimary.update({cityId: primId})
            return(True)

        else:
            selectedTypeId = self.types[0].getId()
            for t in self.types:
                if t.getCap() >= poblation and t.getDist() >= dist:
                    if(self.types[selectedTypeId].getCost()>t.getCost()):
                                selectedTypeId = t.getId()

            self.cityToPrimary.update({cityId: primId})
            self.locationToType.update({primId: selectedTypeId})
            return(True)

        return(False)

    def isFeasibleToAssignCityToLocAsSec(self, cityId, secId):
        if(self.cityToSecondary.has_key(cityId)):
            if(self.verbose): print('City(%s) already has a primary center assigned.' % str(cityId))
            return(False)
        
        city = self.cities[cityId]
        sec = self.locations[secId]
        dist = self.d_cl[cityId,secId]
        poblation = city.getPoblation()

        if(self.locationToType.has_key(secId)):
            cTypeId = self.locationToType[secId]
            cType = self.types[cTypeId]
            cCap = cType.getCap()

            cPrimServed = 0
            for k in self.cityToPrimary.keys():
                if self.cityToPrimary[k] == secId:
                    cPrimServed += self.cities[k].getPoblation()

            cSecServed = 0
            for k in self.cityToSecondary.keys():
                if self.cityToSecondary[k] == secId:
                    cSecServed += self.cities[k].getPoblation()
            
            totalPob = cPrimServed + (cSecServed+poblation)/10

            if(cCap < totalPob or dist > 3*cType.getDist()):
                selectedTypeId = self.types[0].getId()
                isPossible = False
                for t in self.types:
                    if(3*t.getDist() >= dist and t.getCap() >= totalPob):

                        isTpossible = True

                        for k in self.cityToPrimary.keys():
                            if self.d_cl[k,secId] > 3*t.getDist(): 
                                isTpossible = False

                        for k in self.cityToSecondary.keys():
                            if self.d_cl[k,secId] > 3*t.getDist():
                                isTpossible = False

                        if(isTpossible):
                            if(self.types[selectedTypeId].getCost()>t.getCost()):
                                isPossible = True
                                selectedTypeId = t.getId()

                if(isPossible):
                    self.cityToSecondary.update({cityId: secId})
                    self.locationToType[secId] = selectedTypeId
                    return(True)

                return(False)

            self.cityToSecondary.update({cityId: secId})
            return(True)

        else:
            for t in self.types:
                if t.getCap() >= poblation and 3*t.getDist() >= dist:
                    self.cityToSecondary.update({cityId: secId})
                    self.locationToType.update({secId: t.getId()})
                    return(True)

        return(False)

    def getPrimIdAssignedToCityId(self, cityId):
        if(not self.cityToPrimary.has_key(cityId)): return(None)
        return(self.cityToPrimary[cityId])

    def getSecIdAssignedToCityId(self, cityId):
        if(not self.cityToSecondary.has_key(cityId)): return(None)
        return(self.cityToSecondary[cityId])

    def assign(self, cityId, primId, secId):

        # Search if is feasible to do the assigments and if it is does it
        if(not self.isFeasibleToAssignCityToLocAsPrim(cityId, primId)):
            return(False)
        if(not self.isFeasibleToAssignCityToLocAsSec(cityId, secId)):
            return(False)

        self.updateCost()
        return(True)

    def unassign(self, cityId, primId, secId):
        
        del self.cityToPrimary[cityId]
        del self.cityToSecondary[cityId]

        feasibleToUnassignTypeOfPrim = True
        feasibleToUnassignTypeOfSec = True

        for c in self.cityToPrimary.keys():
            if(self.cityToPrimary[c] == primId):
                feasibleToUnassignTypeOfPrim = False
            if(self.cityToPrimary[c] == secId):
                feasibleToUnassignTypeOfSec = False

        for c in self.cityToSecondary.keys():
            if(self.cityToSecondary[c] == primId):
                feasibleToUnassignTypeOfPrim = False
            if(self.cityToSecondary[c] == secId):
                feasibleToUnassignTypeOfSec = False

        if(feasibleToUnassignTypeOfPrim):
            del self.locationToType[primId]

        if(feasibleToUnassignTypeOfSec):
            del self.locationToType[secId]

        self.updateCost()
        return(True)
    
    def getCost(self):
        return(self.cost)
    
    def findFeasibleAssignments(self, cityId):
        startEvalTime = time.time()
        evaluatedCandidates = 0

        feasibleAssignments = []
        for locP in self.locations:
            for locS in self.locations:
                if locP.getId()!=locS.getId() and self.d_ll[locP.getId(),locS.getId()] >= self.d_center:

                    locPId = locP.getId()
                    locSId = locS.getId()
                    feasible = self.assign(cityId, locPId, locSId)
                    
                    evaluatedCandidates += 1
                    if(not feasible): continue
                    
                    assignment = Assignment(cityId, locPId, locSId, self.getCost())
                    feasibleAssignments.append(assignment)
                    
                    self.unassign(cityId, locPId, locSId)
            
        elapsedEvalTime = time.time() - startEvalTime
        return(feasibleAssignments, elapsedEvalTime, evaluatedCandidates)

    def findBestFeasibleAssignment(self, cityId):
        bestAssignment = Assignment(cityId, None, None, float('infinity'))
        for locP in self.locations:
            for locS in self.locations:
                if locP.getId()!=locS.getId() and self.d_ll[locP.getId(),locS.getId()] >= self.d_center:
                    locPId = locP.getId()
                    locSId = locS.getId()
                    feasible = self.assign(cityId, locPId, locSId)
                    
                    if(not feasible): continue
                    
                    curCost = self.getCost()
                    if(bestAssignment.cost > curCost):
                        bestAssignment.primId = locPId
                        bestAssignment.secId = locSId
                        bestAssignment.cost = curCost
                    
                    self.unassign(cityId, locPId, locSId)
            
        return(bestAssignment)
    
    def __str__(self):  # toString equivalent
        nTasks = self.inputData.nTasks
        nThreads = self.inputData.nThreads
        nCPUs = self.inputData.nCPUs
        nCores = self.inputData.nCores
        
        strSolution = 'z = %10.8f;\n' % self.highestLoad
        
        # Xhk: decision variable containing the assignment of threads to cores
        # pre-fill with no assignments (all-zeros)
        xhk = []
        for h in xrange(0, nThreads):   # h = 0..(nThreads-1)
            xhkEntry = [0] * nCores     # results in a vector of 0's with nCores elements
            xhk.append(xhkEntry)

        # iterate over hash table threadIdToCoreId and fill in xhk
        for threadId,coreId in self.threadIdToCoreId.iteritems():
            xhk[threadId][coreId] = 1

        strSolution += 'xhk = [\n'
        for xhkEntry in xhk:
            strSolution += '\t[ '
            for xhkValue in xhkEntry:
                strSolution += str(xhkValue) + ' '
            strSolution += ']\n'
        strSolution += '];\n'
        
        # Xtc: decision variable containing the assignment of tasks to CPUs
        # pre-fill with no assignments (all-zeros)
        xtc = []
        for t in xrange(0, nTasks):     # t = 0..(nTasks-1)
            xtcEntry = [0] * nCPUs      # results in a vector of 0's with nCPUs elements
            xtc.append(xtcEntry)
        
        # iterate over hash table taskIdToCPUId and fill in xtc
        for taskId,cpuId in self.taskIdToCPUId.iteritems():
            xtc[taskId][cpuId] = 1
        
        strSolution += 'xtc = [\n'
        for xtcEntry in xtc:
            strSolution += '\t[ '
            for xtcValue in xtcEntry:
                strSolution += str(xtcValue) + ' '
            strSolution += ']\n'
        strSolution += '];\n'
        
        return(strSolution)

    def saveToFile(self, filePath):
        f = open(filePath, 'w')
        f.write(self.__str__())
        f.close()