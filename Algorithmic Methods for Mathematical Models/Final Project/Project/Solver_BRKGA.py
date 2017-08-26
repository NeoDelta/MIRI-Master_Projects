import random, time
from Solver import Solver
from Solution import Solution

class BRKGA_Individual(object):
    def __init__(self, chromosome, fitness):
        self.chromosome = chromosome
        self.fitness = fitness
        self.solution = None
    
    @classmethod
    def initGreedy(cls, config):
        numGenes = config.numGenes
        
        # generate a chromosome (vector of floats)
        # fill it with random values [0.0 .. 1.0]
        chromosome = []
        for numGene in xrange(0, numGenes):
            chromosome.append(1.0)
        
        # instantiate the new Individual with chromosome and an undefined fitness
        return(cls(chromosome, None))
    
    @classmethod
    def initMutant(cls, config):
        numGenes = config.numGenes
        
        # generate a chromosome (vector of floats)
        # fill it with random values [0.0 .. 1.0]
        chromosome = []
        for numGene in xrange(0, numGenes):
            chromosome.append(random.uniform(0, 1))
        
        # instantiate the new Individual with chromosome and an undefined fitness
        return(cls(chromosome, None))
    
    @classmethod
    def initCrossOver(cls, elite, nonElite, config):
        numGenes = config.numGenes
        pInheritanceElite = config.pInheritanceElite
        
        # chromosomes from parents (elite and non-elite) must have the same size to be crossed
        if(len(elite.chromosome) != len(nonElite.chromosome)):
            raise Exception('Unable to cross-over individuals with different chromosome size')
        
        # generate a chromosome (vector of floats)
        # initialize the chromosome by crossing over the parents
        chromosome = []
        for numGene in xrange(0, numGenes):
            # pick genes from parents
            eliteGen = elite.chromosome[numGene]
            nonEliteGen = nonElite.chromosome[numGene]
            
            # pick the gene from elite or non-elite
            # pInheritanceElite: probability of picking the gene from the elite parent
            pInheritance = random.uniform(0.0, 1.0);
            if(pInheritance < pInheritanceElite):
                gene = eliteGen
            else:
                gene = nonEliteGen
            chromosome.append(gene)
        
        # instantiate the new Individual with chromosome and an undefined fitness
        return(cls(chromosome, None))

# An abstract BRKGA decoder
class BRKGA_Decoder(object):
    def __init__(self):
        raise Exception('Abstract method cannot be called')
    
    def getNumGenes(self):
        raise Exception('Abstract method cannot be called')
    
    def decode(self, individual):
        raise Exception('Abstract method cannot be called')

# Inherits from a parent abstract solver.
class Solver_BRKGA(Solver):
    
    def initializeIndividuals(self, config):
        # create a population with numIndividuals individuals created at random
        population = []
        
        individual = BRKGA_Individual.initGreedy(config)
        population.append(individual)
        
        for numIndividual in xrange(1, config.numIndividuals):
            individual = BRKGA_Individual.initMutant(config)
            population.append(individual)
        return(population)
    
    def decodeIndividuals(self, population, decoder):
        # decode individuals not already decoded, i.e. with undefined fitness, with a specific decoder
        it_decodedIndividuals = 0
        startTime = time.time()
        
        for individual in population:
            if(individual.fitness is not None): continue
            decoder.decode(individual)
            it_decodedIndividuals += 1
        
        it_elapsedDecodingTime = time.time() - startTime
        return(it_elapsedDecodingTime, it_decodedIndividuals)
    
    def sortIndividuals(self, population):
        # sort individuals in a population by their fitness in ascending order
        population.sort(key=lambda individual: individual.fitness)
    
    def evolveIndividuals(self, population, config):
        numElites = config.numElites
        
        # get elites and non-elites from current population
        elites = population[0:numElites]            # elites: sublist from 0 to (numElites-1)
        nonElites = population[numElites:]          # nonElites: sublist from numElites to the end

        newPopulation = []
        
        # direct copy the numElites elite individuals to the new population
        newPopulation[0:numElites] = elites
        
        # create numCrossOvers individuals by crossing randomly selected parents
        for numCrossOver in xrange(0, config.numCrossOvers):
            elite = random.choice(elites)           # pick an elite individual at random
            nonElite = random.choice(nonElites)     # pick a non-elite individual at random
            
            # crossover them parents (elite and non-elite) to produce a new individual
            # pick each gene from elite or non-elite with a specific inheritance probability 
            individual = BRKGA_Individual.initCrossOver(elite, nonElite, config)
            newPopulation.append(individual)
        
        # create numMutants individuals are created at random
        for numMutant in xrange(0, config.numMutants):
            individual = BRKGA_Individual.initMutant(config)
            newPopulation.append(individual)
        
        return(newPopulation)
    
    def solve(self, config, problem, decoder):
        bestSolution = Solution.createEmptySolution(config, problem)
        bestSolution.makeInfeasible()
        bestCost = bestSolution.getCost()
        
        self.startTimeMeasure()
        self.writeLogLine(bestCost, 0)
        
        total_elapsedDecodingTime = 0
        total_decodedIndividuals = 0
        
        # get number of individuals and number of genes per chromosome
        config.numGenes = decoder.getNumGenes()
        config.numIndividuals = int(round(decoder.getNumIndividuals()))

        # pre-compute number of elite, crossover and mutants in the population
        config.numElites = int(round(config.pElites * config.numIndividuals))
        config.numMutants = int(round(config.pMutants * config.numIndividuals))
        config.numCrossOvers = config.numIndividuals - config.numElites - config.numMutants
        
        if(config.numElites <= 0):
            raise Exception('pElites(%s) results in an invalid numElites(%s).' % (config.pElites, config.numElites))

        if(config.numMutants <= 0):
            raise Exception('pMutants(%s) results in an invalid numMutants(%s).' % (config.pMutants, config.numMutants))

        if(config.numCrossOvers <= 0):
            raise Exception('pElites(%s) and pMutants(%s) results in an invalid numCrossOvers(%s).' % (config.pElites, config.pMutants, config.numCrossOvers))
        
        population = self.initializeIndividuals(config)
        
        generation = 0
        while(time.time() - self.startTime < config.maxExecTime):
            generation += 1
            
            # decode the individuals using the decoder
            it_elapsedDecodingTime, it_decodedIndividuals = self.decodeIndividuals(population, decoder)
            total_elapsedDecodingTime += it_elapsedDecodingTime
            total_decodedIndividuals += it_decodedIndividuals
            
            # sort them by their fitness
            self.sortIndividuals(population)
            
            # update statistics
            bestIndividual = population[0]
            newBestCost = bestIndividual.fitness
            if(newBestCost < bestCost):
                bestSolution = bestIndividual.solution
                bestCost = newBestCost
                self.writeLogLine(bestCost, generation)
            
            # evolve the population
            population = self.evolveIndividuals(population, config)
        
        self.writeLogLine(bestCost, generation)
        
        avg_decodingTimePerIndividual = 0.0
        if(total_decodedIndividuals != 0):
            avg_decodingTimePerIndividual = 1000.0 * total_elapsedDecodingTime / float(total_decodedIndividuals)
        
        print ''
        print 'BRKGA Individual Decoder Performance:'
        print '  Num. Individuals Decoded', total_decodedIndividuals
        print '  Total Decoding Time     ', total_elapsedDecodingTime, 's'
        print '  Avg. Time / Individual  ', avg_decodingTimePerIndividual, 'ms'
        
        return(bestSolution)