/*********************************************
 * OPL 12.7.0.0 Model
 * Author: Deme
 * Creation Date: 22/5/2017 at 0:04:52
 *********************************************/

 int nLocations = ...;
 int nCities = ...;
 int nTypes = ...;
 
 range L = 1..nLocations;
 range C = 1..nCities;
 range T = 1..nTypes;
 
 int posCities[c in C][i in 1..2] = ...;
 int posLocations[l in L][i in 1..2] = ...;
 int p[c in C] = ...;
 
 // For type of center
 float d_city[t in T] = ...; // max distance connection to cities
 int cap[t in T] = ...;    // capacity
 int cost[t in T] = ...;   // cost of installing
 
 float d_center = ...; // max distance between two centers
 
 float d_cl[c in C][l in L];   // distance between cities and locations
 float d_ll[l1 in L][l2 in L]; // distance between locations
 
 float M = 10000;
 // These decision variables should suffice
 dvar boolean prim[c in C][l in L]; // whether city c is served by a center in location l
 dvar boolean sec[c in C][l in L];
 dvar boolean inst[l in L][t in T]; // whether a center of type t is installed in location l
 
 execute { 
  
    for (var c in C)
  		for (var l in L)
  			d_cl[c][l] = Opl.sqrt( Opl.pow(posCities[c][1]-posLocations[l][1], 2) + 
  						 		   Opl.pow(posCities[c][2]-posLocations[l][2], 2));
	
	for (var l1 in L)
 		for (var l2 in L)
  			d_ll[l1][l2] =  Opl.sqrt( Opl.pow(posCities[l1][1]-posLocations[l2][1], 2) + 
  						              Opl.pow(posCities[l1][2]-posLocations[l2][2], 2));
 }
 
 // Objective function
 minimize sum(l in L, t in T) inst[l][t]*cost[t];
 
 subject to {
    
  // Each city to exactly one primary
  forall (c in C)
    	sum (l in L) prim[c][l] == 1;
  
  // Each city to exactly one secondary
  forall (c in C)
    	sum (l in L) sec[c][l] == 1;
  
  // Primary and secondary are different
  forall (c in C, l in L)
  		prim[c][l] + sec[c][l] <= 1;
  		
  
  forall(l in L, c in C)
    (prim[c][l]+sec[c][l])-sum(t in T) inst[l][t] <= 0;
  		
  // One center or less per location
  forall (l in L)
    	sum (t in T) inst[l][t] <= 1;
  		
  // Solved
  forall (l1,l2 in L, t1,t2 in T)
    	d_ll[l1][l2] >= d_center - M*(1-(inst[l1][t1] + inst[l2][t2] -1));
    	//(inst[l1][t1] + inst[l2][t2] -1) * d_ll[l1][l2] <= d_center;
    	 
  // Solved
  forall (l in L, t in T)
    	cap[t] >= ((sum (c in C) p[c]*prim[c][l]) + sum (c in C) p[c]*sec[c][l] / 10 ) - M*(1-inst[l][t]);
   		/*cap[t] >= (sum (c in C) p[c]*(inst[l][t] + prim[c][l] -1) + 
   		          (sum (c in C) p[c]*(inst[l][t] + sec[c][l] -1)) / 10 );*/
   		
  // Maximum distances between a city and its centers	
  forall ( c in C, t in T, l in L)
    d_cl[c][l]*(inst[l][t] + prim[c][l] -1) <= d_city[t];
    
  forall ( c in C, t in T, l in L)
    d_cl[c][l]*(inst[l][t] + sec[c][l] -1) <= 3*d_city[t];
 }
 
 execute { 
 
  	writeln("Finished");
  	
  	var typeOfLocation = new Array(nLocations+1);
  	for (var l in L) {
  		typeOfLocation[l] = 0;
  		for (var t in T) {
  			if (inst[l][t] == 1) typeOfLocation[l] = t; 	  
    	}  		        	
  	}
  	
  	for (l in L) {
  	  	if (typeOfLocation[l] != 0) {
			write("Location " + l + " has a center of type " + typeOfLocation[l] + ".");
  			var total = 0;
  			for (var c in C)
	  			if (prim[c][l] == 1) total += p[c];
			writeln(" It serves " + total + " inhabitants, max is " + cap[typeOfLocation[l]] + ".");
 		}			        		                  	  					
   	}  			
   
   writeln("");
   
   for (var l1 in L)
   	for (var l2 in L) {
   		if (l1 < l2 && typeOfLocation[l1] != 0 && typeOfLocation[l2] != 0) {
   			write("Locations " + l1 + " and " + l2 + " are used and distance is " + d_ll[l1][l2] + ". ");
   			writeln("Min is " + d_center + ".");
    	}   			
   	}   			
   writeln("");
	  			 
   for (c in C)
  		for (l in L)
 			if (prim[c][l] == 1) {
				if (typeOfLocation[l] == 0) {writeln("Error: city " + c + " is connected to location " + l + " which has no center"); break;}
 			  	writeln("City " + c + " is served by center at location " + l + " (distance " + d_cl[c][l] + ", max is " + d_city[typeOfLocation[l]] + ").");
 			}    	  	
  
   writeln("");

 }      