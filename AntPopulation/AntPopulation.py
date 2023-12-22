import csv
import matplotlib.pyplot as plt
import re
import os
import pathlib


prev_death = 0
correctIteration = False
def countAntState(Count, AliveAnt, DeadAnt, BirthAnt, EachGenFile, AAA, DAA, BAA, Iteration, PERFORMANCE_MODE):
    global correctIteration
    global prev_death
    line = "placeholder"
    while(line != ''): 
        line = EachGenFile.readline()
        checkIteration = re.search("ITERATION", line) #check iteration so it reads the correct one not all
        if (checkIteration != None) and (checkIteration.group() == "ITERATION"):
            if int(re.split("ITERATION: ", line)[1]) == Iteration:
                correctIteration = True
            else:
                correctIteration = False
        if correctIteration:
            checkAlive = re.search("Alive", line) #check how many are alive
            checkDead = re.search("Dead", line) #check how many died this gen
            checkBirth = re.search("Birth", line) #check how many are birthing
            if (checkAlive != None) and (checkAlive.group() == "Alive"):
                AliveAnt += 1
            if (checkBirth != None) and (checkBirth.group() == "Birth"):
                BirthAnt += 1
            elif (checkDead != None) and (checkDead.group() == "Dead"):
                DeadAnt += 1
    if prev_death > DeadAnt:
        prev_death = 0  
    #put alive, birth, dead ant info to arrays
    AAA[Count] = AliveAnt
    BAA[Count] = BirthAnt
    DAA[Count] = DeadAnt - prev_death
    if PERFORMANCE_MODE == "0":
        prev_death = DeadAnt

def main():
    
    with open(r"c:\users\aurij\source\repos\artant\StartData.txt", newline='') as csvfile: #import starting information from ant simulation
        spamreader = csv.reader(csvfile, delimiter=' ')
        listlength = 0
        Table = [None] * 8
        for row in spamreader:
            Table[listlength] = row[1]
            listlength += 1
    START_ANT = Table[0] #starting ant amount
    START_FOOD = Table[1] #starting food amount
    NEW_FOOD = Table[2] #food amount can be spawned when food is eaten
    ANT_SPEED = Table[3] #speed of ant
    STARVE = Table[4] #every how many generations to decrease food
    ITERATIONS = int(Table[5]) #iterations of simulation = amount of graphs
    GenFileAmount = int(Table[6]) #amount of generations per iteration
    PERFORMANCE_MODE = Table[7] #when on it doesnt save dead ants only new deaths
    AvrgAlive = [0] * ITERATIONS
    AvrgDead = [0] * ITERATIONS
    AvrgBirth = [0] * ITERATIONS
    for iter in range(1, ITERATIONS + 1, 1):
        Generation = list(range(0, GenFileAmount + 1)) #generation axis 
        AliveAntsArray = [int(START_ANT)] * (GenFileAmount + 1) #amount of alive ants 
        DeadAntsArray = [0] * (GenFileAmount + 1) #amount of dieing ants 
        BirthAntsArray = [0] * (GenFileAmount + 1) #amount of birthing ants 
        
        for Gen in range(1, GenFileAmount + 1, 1): #cycle each gen
            AliveAnts = 0
            DeadAnts = 0
            BirthAnts = 0
            if Gen < 10:
                GenFile = open(r"c:\users\aurij\source\repos\artant\Gen_00{}.txt".format(Gen), "r")
            elif Gen >= 10:
                GenFile = open(r"c:\users\aurij\source\repos\artant\Gen_0{}.txt".format(Gen), "r")
            countAntState(Gen, AliveAnts, DeadAnts, BirthAnts, GenFile, AliveAntsArray, DeadAntsArray, BirthAntsArray, iter, PERFORMANCE_MODE)
            GenFile.close()
        AvrgAlive[iter - 1] = AliveAntsArray
        AvrgDead[iter - 1] = DeadAntsArray
        AvrgBirth[iter - 1] = BirthAntsArray
        plt.clf()
        plt.plot(Generation, AliveAntsArray, color='g', label ='Alive')
        plt.plot(Generation, DeadAntsArray, 'r--', label='New Deaths')
        plt.plot(Generation, BirthAntsArray, 'b--', label = 'New Births')
        plt.xticks(range(min(Generation), max(Generation) + 1, 2))
        plt.xlabel('Ant Generations')
        plt.ylabel('Ant State')
        plt.legend(loc = 'upper left')
        plt.title('Ant Population') 
        plt.grid(True)
        #plt.savefig(r'C:\Users\aurij\source\repos\ArtAnt\GraphsFolder\A{} F{} NF{} AS{} S{} I{}.png'.format(START_ANT, START_FOOD, NEW_FOOD, ANT_SPEED, STARVE, iter))
        plt.show()
    for Gen in range(GenFileAmount + 1):
        A = 0
        D = 0
        B = 0
        for iter in range(ITERATIONS):
            A += AvrgAlive[iter][Gen] 
            D += AvrgDead[iter][Gen] 
            B = AvrgBirth[iter][Gen]
        AvrgAlive[0][Gen] = round(A / ITERATIONS, 1)
        AvrgDead[0][Gen] = round(D / ITERATIONS, 1)
        AvrgBirth[0][Gen] = round(B / ITERATIONS, 1)
    plt.clf()
    plt.plot(Generation, AvrgAlive[0], color='g', label ='Average Alive')
    plt.plot(Generation, AvrgDead[0], 'r--', label='Average New Deaths')
    plt.plot(Generation, AvrgBirth[0], 'b--', label = 'Average New Births')
    plt.xticks(range(min(Generation), max(Generation) + 1, 2))
    plt.xlabel('Ant Generations')
    plt.ylabel('Ant State')
    plt.legend(loc = 'upper left')
    plt.title('Average Ant Population')
    plt.grid(True)
    plt.savefig(r'C:\Users\aurij\source\repos\ArtAnt\GraphsFolder\Avrg A{} F{} NF{} AS{} S{} TI{}.png'.format(START_ANT, START_FOOD, NEW_FOOD, ANT_SPEED, STARVE, ITERATIONS))
    plt.show()
main()