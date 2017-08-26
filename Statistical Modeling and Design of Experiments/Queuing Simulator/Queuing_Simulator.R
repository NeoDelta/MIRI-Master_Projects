#library(mixdist)

congRNG <- function(mult, inc, seed, mod, num){
   
   aux = 0.0
   output = 0.0
   result = numeric()
   
   for ( i in 1:num){
     aux = (mult*seed+inc) %% mod
     output = aux/mod
     result[i] = output
     seed = aux
   }
   
   return(result)
 }
 
res <- congRNG(16807, 0, 12540, 2^31-1, 100000)

uniformRVG <- function(randNum, lo, up){
  if(up <= lo ) stop("up must be greater than lo")
  return(randNum*(up-lo)+lo)
}


eT = 0
t = 0
iter = 100000
niter = 100000

Lq = 0
L = 0
Wq = 0
W = 0

# for p = 0.4 use b = 22.525
# for p = 0.7 use b = 39.4
# for p = 0.85 use  b = 47.8525
# for p = 0.925 use b = 52.075
b = 22.525

data <- data.frame(numeric(),numeric())

while(iter>0){
  ts = max(eT,t)
  
  x = rweibull(1, 0.7250, b)
  eT = ts + x
  
  L = L + eT - t
  Lq = Lq + ts - t
  Lt = L/t
  W = W + eT - t
  Wq = Wq + ts - t
  
  if(iter<niter){
    aux <- data.frame(t, Lt)
    data<-rbind(aux,data)
  }
  
  if(iter%%(niter/10)==0){
    cat("L: ", L," Lq: ", Lq," Lt: ", Lt," W: ",W," Wq: ",Wq, "\n")
  }
  
  if(iter > 0){
    tau = uniformRVG(res[iter], 0, 138)
    t = t + tau
  }
  
  if(iter == niter) fT = t
  iter = iter - 1
}

W = W/niter
Wq = Wq/niter
L = L/(t-fT)
Lq = Lq/(t-fT)

#mx = mean(data[["x"]])
#aux2 <- data.frame(W,Wq,L,Lq,mx)
#datamx<-rbind(aux2,datamx)

cat("L: ", L," Lq: ", Lq," W: ",W," Wq: ",Wq, "\n")
plot(data)
