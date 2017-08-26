import random, time, copy
from Solver import Solver
from Solution import Solution
from LocalSearch import LocalSearch

# Inherits from a parent abstract solver.
class Solver_GRASP(Solver):
    def selectCandidate(self, config, candidateList):
        if(len(candidateList) == 0): return(None)
        
        # sort candidate assignments by highestLoad in ascending order
        # PUEDE QUE HAYA QUE CAMBIAR EL REVERSE
        sortedCL = sorted(candidateList, key=lambda candidate: candidate.cost, reverse=False)
        
        # compute boundary highest load as a function of the minimum and maximum highest loads and the alpha parameter
        alpha = config.alpha
        minCost = sortedCL[0].cost
        maxCost = sortedCL[len(sortedCL)-1].cost
        boundaryCost = minCost + (maxCost - minCost) * alpha
        
        # find elements that fall into the RCL (those fulfilling: highestLoad < boundaryHighestLoad)
        maxIndex = 0
        for x in sortedCL:
            if(x.cost > boundaryCost): break
            maxIndex += 1

        # create RCL and pick an element randomly
        rcl = sortedCL[0:maxIndex]          # pick first maxIndex elements starting from element 0
        if(len(rcl) == 0): return(None)
        return(random.choice(rcl))          # pick an element from rcl at random
    
    def greedyRandomizedConstruction(self, config, problem):
        # get an empty solution for the problem
        solution = Solution.createEmptySolution(config, problem)

        iteration_elapsedEvalTime = 0
        iteration_evaluatedCandidates = 0
        
        # get tasks and sort them by their total required resources in ascending order
        cities = problem.getCities()
        sortedCities = sorted(cities, key=lambda city: city.getPoblation(), reverse=False)

        # for each task taken in sorted order
        for c in range(len(sortedCities)):
            cityId = sortedCities[c].getId()
            
            # compute feasible assignments
            candidateList, elapsedEvalTime, evaluatedCandidates = solution.findFeasibleAssignments(cityId)
            iteration_elapsedEvalTime += elapsedEvalTime
            iteration_evaluatedCandidates += evaluatedCandidates
            
            # no candidate assignments => no feasible assignment found
            if(len(candidateList) == 0):
                solution.makeInfeasible()
                break
            
            # select an assignment
            candidate = self.selectCandidate(config, candidateList)
            if(candidate is None): break

            # assign the current task to the CPU that resulted in a minimum highest load
            solution.assign(cityId, candidate.primId, candidate.secId)
         
        return(solution, iteration_elapsedEvalTime, iteration_evaluatedCandidates)
    
    def solve(self, config, problem):
        bestSolution = Solution.createEmptySolution(config, problem)
        bestSolution.makeInfeasible() 
        bestCost = bestSolution.getCost()
        self.startTimeMeasure()
        self.writeLogLine(bestCost, 0)
        
        total_elapsedEvalTime = 0
        total_evaluatedCandidates = 0
        
        localSearch = LocalSearch(config)
        
        iteration = 0
        while(time.time() - self.startTime < config.maxExecTime):
            iteration += 1
            
            # force first iteration as a Greedy execution (alpha == 0)
            originalAlpha = config.alpha 
            if(iteration == 1): config.alpha = 0
            
            solution, it_elapsedEvalTime, it_evaluatedCandidates = self.greedyRandomizedConstruction(config, problem)
            total_elapsedEvalTime += it_elapsedEvalTime
            total_evaluatedCandidates += it_evaluatedCandidates
            
            # recover original alpha
            if(iteration == 1): config.alpha = originalAlpha
            
            if(not solution.isFeasible()): continue
            solution = localSearch.run(solution)
            solutionCost = solution.getCost()
            if(solutionCost < bestCost):
                bestSolution = solution
                bestCost = solutionCost
                self.writeLogLine(bestCost, iteration)
            
        self.writeLogLine(bestCost, iteration)

        for k in bestSolution.cityToPrimary.keys():
            print 'City ', k, ' has primary ', bestSolution.cityToPrimary[k]

        for k in bestSolution.cityToSecondary.keys():
            print 'City ', k, ' has secondary ', bestSolution.cityToSecondary[k]

        for k in bestSolution.locationToType.keys():
            print 'Location ', k, ' has type ', bestSolution.locationToType[k]
        
        avg_evalTimePerCandidate = 0.0
        if(total_evaluatedCandidates != 0):
            avg_evalTimePerCandidate = 1000.0 * total_elapsedEvalTime / float(total_evaluatedCandidates)
        
        print ''
        print 'GRASP Candidate Evaluation Performance:'
        print '  Num. Candidates Eval.', total_evaluatedCandidates
        print '  Total Eval. Time     ', total_elapsedEvalTime, 's'
        print '  Avg. Time / Candidate', avg_evalTimePerCandidate, 'ms'
        
        localSearch.printPerformance()
        
        return(bestSolution)