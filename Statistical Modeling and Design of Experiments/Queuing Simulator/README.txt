--------------------------------------------------------
	Statistical Modeling and Design of Experiment
	3rd Assigment - Queueign Simulator 
	Diego Campos Milian
--------------------------------------------------------
-------Instructions-------------------------------------

	- To execute on Linux terminal with: 
		RScript 3rdAssigment.R

	- To execute on windows:
		Compile directly from R-Studio

	In both cases you may need a copy of R installed
		
	Once executed the program will simulate the
	stablished model in the assigment with 100000
	individuals and will print W, Wq, Lq and L values
	every 10000 individuals. Finally it will shows 
	the final results and print a graphic of the 
	evolution of the occupation over time.
	
	Note that due to do amount of individuals and
	values to stored the execution may take a couple
	of minutes. To increase speed consider comment 
	lines from 57 to 60, both included.

	The code will execute by default with and objective
	loading factor of 0.4. To change this, and any other
	variable it is needed to do it directly on the code.
	b values for specific loading factors are already 
	provided.
--------------------------------------------------------