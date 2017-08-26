from Solver_BRKGA import BRKGA_Decoder
from Solution import Solution

# The specific decoder for Task to CPU Assignment problem that implements methods in BRKGA_Decoder
# This decoder sorts the tasks to be assigned to CPUs according to a priority.
# The lower the priority, the earlier the task is assigned.
# Each gene in a chromosome represents the priority of a task for a specific individual.
# The decoder sorts the tasks acording to the priorities in the chromosome and
# tries to assign the tasks to CPUs following that order.
class CityToCentersAssignment_Decoder(BRKGA_Decoder):
    def __init__(self, config, problem):
        self.config = config
        self.problem = problem
        
        numCities = len(self.problem.getCities())
        self.numIndividuals = numCities * self.config.fIndividuals
        self.numGenes = numCities
    
    def getNumIndividuals(self):
        return(self.numIndividuals)
    
    def getNumGenes(self):
        return(self.numGenes)

    def decode(self, individual):
        # get tasks and genes
        cities = self.problem.getCities()     # vector of tasks [t1, t2, t3, ...]
        genes = individual.chromosome       # vector of genes [g1, g2, g3, ...]
        
        # zip vector tasks and vector genes in a vector of tuples with the form:
        # [(t1,g1), (t2,g2), (t3,g3), ...]
        cities_and_genes = zip(cities, genes)
        
        # sort the vector of tuples tasks_and_genes by the genes (index 1 in the tuple)
        cities_and_genes.sort(key=lambda city_and_gene: city_and_gene[0].getPoblation() * city_and_gene[1], reverse=True)
        
        # create an empty solution for the individual
        individual.solution = Solution.createEmptySolution(self.config, self.problem)
        
        # assign each task following the given order
        for city_and_gene in cities_and_genes:
            city = city_and_gene[0]
            cityId = city.getId()
            
            # get the feasible assignment for the current task with minimum highest CPU load
            assignment = individual.solution.findBestFeasibleAssignment(cityId)
            bestPrimId = assignment.primId
            bestSecId = assignment.secId
            
            if(bestPrimId is None or bestSecId is None):
                # infeasible solution
                individual.solution.makeInfeasible()
                break
            
            # do assignment
            individual.solution.assign(cityId, assignment.primId, assignment.secId)
        
        individual.fitness = individual.solution.getCost()