import copy, time

# A change in a solution in the form: move taskId from curCPUId to newCPUId.
# This class is used to carry sets of modifications.
# A new solution can be created based on an existing solution and a list of
# changes can be created using the createNeighborSolution(solution, changes) function.
class Change(object):
    def __init__(self, cityId, curPrimId, newPrimId, curSecId, newSecId):
        self.cityId = cityId
        self.curPrimId = curPrimId
        self.newPrimId = newPrimId
        self.curSecId = curSecId
        self.newSecId = newSecId

# Implementation of a local search using two neighborhoods and two different policies.
class LocalSearch(object):
    def __init__(self, config):
        self.enabled = config.localSearch
        self.nhStrategy = config.neighborhoodStrategy
        self.policy = config.policy
        
        self.elapsedTime = 0
        self.iterations = 0

    def createNeighborSolution(self, solution, changes):
        # first unassign the tasks specified in changes
        # then reassign them to the new CPUs
        
        newSolution = copy.deepcopy(solution)
        
        for change in changes:
            newSolution.unassign(change.cityId, change.curPrimId, change.curSecId)
        
        for change in changes:
            feasible = newSolution.assign(change.cityId, change.newPrimId, change.newSecId)
            if(not feasible): return(None)
        
        return(newSolution)
    
    
    def evaluateNeighbor(self, solution, changes):
        curSolution = copy.deepcopy(solution)
        curCost = curSolution.getCost();

        for change in changes:
            newSolution = copy.deepcopy(curSolution)
            newSolution.unassign(change.cityId, change.curPrimId, change.curSecId)
            feasible = newSolution.assign(change.cityId, change.newPrimId, change.newSecId)

            if(not feasible): continue

            newCost = newSolution.getCost()
            if newCost < curCost:
                curCost = newCost
            
        return(curCost)
    
    def getCitiesAssignmentsSortedByRelativeCost(self, solution):
        cities = solution.getCities()
        locs = solution.getLocations()
        
        # create vector of task assignments.
        # Each entry is a tuple (task, cpuAssigned) 
        citiesAssignments = []
        for city in cities:
            cityId = city.getId()
            primId = solution.getPrimIdAssignedToCityId(cityId)
            secId = solution.getSecIdAssignedToCityId(cityId)
            prim = locs[primId]
            sec = locs[secId]

            citsServedByPrim = 0
            citsServedBySec = 0

            for k in solution.cityToPrimary.keys():
                if solution.cityToPrimary[k] == primId:
                    citsServedByPrim += 1
                if solution.cityToPrimary[k] == secId:
                    citsServedBySec += 1

            for k in solution.cityToSecondary.keys():
                if solution.cityToSecondary[k] == primId:
                    citsServedByPrim += 1
                if solution.cityToSecondary[k] == secId:
                    citsServedBySec += 1

            primType = solution.locationToType[primId]
            secType = solution.locationToType[secId]

            relativeCost = solution.types[primType].getCost()/citsServedByPrim + solution.types[secType].getCost()/citsServedBySec

            cityAssignment = (city, prim, sec, relativeCost)
            citiesAssignments.append(cityAssignment)

        # For best improvement policy it does not make sense to sort the tasks since all of them must be explored.
        # However, for first improvement, we can start by tasks in most loaded CPUs.
        if(self.policy == 'BestImprovement'): return(citiesAssignments)
        
        # Sort task assignments by the load of the assigned CPU in descending order.
        sorted_citiesAssignments = sorted(citiesAssignments, key=lambda cityAssignment:cityAssignment[3], reverse=True)
        return(sorted_citiesAssignments)
    
    def exploreNeighborhood(self, solution):
        locs = solution.getLocations()
        
        curCost = solution.getCost()
        bestNeighbor = solution
        
        if(self.nhStrategy == 'Reassignment'):
            sortedCitiesAssignments = self.getCitiesAssignmentsSortedByRelativeCost(solution)
                
            for cityAssignment in sortedCitiesAssignments:
                city = cityAssignment[0]
                cityId = city.getId()
                
                curPrim = cityAssignment[1]
                curPrimId = curPrim.getId()

                curSec = cityAssignment[2]
                curSecId = curSec.getId()
                
                for pLoc in locs:
                    newPrimId = pLoc.getId()
                    if(newPrimId == curPrimId): continue
                    for sLoc in locs:
                        newSecId = sLoc.getId()
                        if(newSecId == curSecId): continue
                        if(newSecId == newPrimId): continue
                        
                        changes = []
                        changes.append(Change(cityId, curPrimId, newPrimId, curSecId, newSecId))
                        neighborCost = self.evaluateNeighbor(solution, changes)

                        if(curCost > neighborCost):
                            neighbor = self.createNeighborSolution(solution, changes)
                            if(neighbor is None): continue
                            if(self.policy == 'FirstImprovement'):
                                return(neighbor)
                            else:
                                bestNeighbor = neighbor
                                curCost = neighborCost
        return(bestNeighbor)
    
    def run(self, solution):
        if(not self.enabled): return(solution)
        if(not solution.isFeasible()): return(solution)

        bestSolution = solution
        bestCost = bestSolution.getCost()
        
        startEvalTime = time.time()
        iterations = 0
        
        # keep iterating while improvements are found
        keepIterating = True
        while(keepIterating):
            keepIterating = False
            iterations += 1
            
            neighbor = self.exploreNeighborhood(bestSolution)
            curCost = neighbor.getCost()
            if(bestCost > curCost):
                bestSolution = neighbor
                bestCost = curCost
                keepIterating = True
        
        self.iterations += iterations
        self.elapsedTime += time.time() - startEvalTime
        
        return(bestSolution)
    
    def printPerformance(self):
        if(not self.enabled): return
        
        avg_evalTimePerIteration = 0.0
        if(self.iterations != 0):
            avg_evalTimePerIteration = 1000.0 * self.elapsedTime / float(self.iterations)
        
        print ''
        print 'Local Search Performance:'
        print '  Num. Iterations Eval.', self.iterations
        print '  Total Eval. Time     ', self.elapsedTime, 's'
        print '  Avg. Time / Iteration', avg_evalTimePerIteration, 'ms'