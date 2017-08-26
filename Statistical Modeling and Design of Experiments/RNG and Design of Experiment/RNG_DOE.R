library(randtests)

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

res <- congRNG(16807, 0, 48271, 2^31-1, 10000)
bartels.rank.test(res)
rank.test(res)

uniformRVG <- function(randNum, lo, up){
  if(up <= lo ) stop("up must be greater than lo")
  return(randNum*(up-lo)+lo)
}

expRVG <- function(randNum, lo, up){
  if(up <= 0 ) stop("up must be greater than 0")
  return(lo-up*log(randNum))
}

powRVG <- function(randNum, n, lo, up){
  if(up <= 0 ) stop("up must be greater than 0")
  if(n <= 0 ) stop("n must be greater than 0")
  return(lo+(up-lo)*(randNum^(1/n)))
}

logarithmRVG <- function(randNum, lo, up){
  if(up <= lo ) stop("up must be greater than lo")
  return(randNum*randNum*(up-lo)+lo)
}

logisticRVG <- function(randNum, lo, up){
  if(up <= 0 ) stop("up must be greater than lo")
  return(lo-up*log10(1/randNum-1))
}

rvu <- uniformRVG(res[1], 0, 100)
rvexp <- expRVG(res[1], 0, 100)

data <- data.frame(numeric(),numeric(),numeric(),numeric(),numeric(),numeric(),numeric(),numeric(),numeric(),numeric(),numeric())
names(data)<-c("F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","Answer")

for( i in 1:1000){
  F1 = uniformRVG(res[i], 80, 250)
  F2 = expRVG(res[i], 0, 100)
  F3 = powRVG(res[i], 10, 10, 50)
  F4 = logarithmRVG(res[i], 20, 180)
  F5 = logisticRVG(res[i], 5, 80)
  F6 = 2*F1
  F7 = F1+3*F2
  F8 = F4+F5
  F9 = F3-F5
  F10 = F4/F2
  # Factors and answer
  aux <- data.frame(
    uniformRVG(res[i], 80, 250),
    expRVG(res[i], 0, 100),
    powRVG(res[i], 10, 10, 50),
    logarithmRVG(res[i], 20, 180),
    logisticRVG(res[i], 5, 80),
    F6,
    F7,
    F8,
    F9,
    F10,
    F6+F7+F8+F9+rnorm(1, mean=0, sd=30))
  names(aux)<-c("F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","Answer")

  data<-rbind(aux,data)
}

data2 <- data.matrix(data)

p1 <- princomp( data, cor = TRUE)
summary(p1)
loadings(p1)
plot(p1)
biplot(p1)
p1$scores

p2 <- prcomp(data, scale = TRUE)
print(p2)
plot(p2)
biplot(p2)

regModels <- list()
max = 0.0
min = 100
for( i in names(data)){
  for( j in names(data)){
    for( t in names(data)){
      for( q in names(data)){
        if( i != "Answer" && i!=j && i!=t && i!=q && j!=t && j!=q && t!=q && j!="Answer" && t!="Answer" && q!="Answer"){
          #if( i != "X1500m" && i!=j && i!=t && j!=t && j!="X1500m" && t!="X1500m"){
          # print(i)
          x <- data[[i]]
          x2 <- data[[j]]
          x3 <- data[[t]]
          x4 <- data[[q]]
          #regModels[[i]] <- lm(X1500m~x+x2+x3, data=decathlon)
          
          RegModel <- lm(Answer~x+x2+x3+x4, data=data)
          rs <- summary(RegModel)$adj.r.squared
          p<-summary(RegModel)$sigma
          #p <- pf(f[1],f[2],f[3],lower.tail=F)
          
          if(rs>max){
            max=rs
            best <- RegModel
            best2 <- list(i,j,t,q)
          }
          if(p<min){
            min=p
            bestp <- RegModel
            best2p <- list(i,j,t,q)
          }
          #print(summary(regModels[[i]]))
        }
      }
    }
  }
}

LM <- lm(Answer~F1+F2+F4, data=data)
#LM <- lm(Answer~F4+F7+F2+F8, data=data)
modData <- data[751:1000,]
yates <- data.frame(A=1:16,B=1:16,C=1:16,D=1:16,answer=1:16,x1=1:16,x2=1:16,x3=1:16,x4=1:16,effect=1:16)
it=0
for(i in 0:1){
  for(j in 0:1){
    for(k in 0:1){
      for( t in 0:1){
        it=it+1
        D = max(modData["F3"])
        C = max(modData["F4"])
        B = max(modData["F2"])
        A = max(modData["F1"])
        if(i==0) D = min(modData["F3"])
        if(j==0) C = min(modData["F4"])
        if(k==0) B = min(modData["F2"])
        if(t==0) A = min(modData["F1"])
        
        aux = data.frame(F3=D,
                          F4=C,
                          F2=B,
                          F1=A)
        
        pred <- predict(LM, aux, interval="confidence")
        
        yates[it,1] <- t
        yates[it,2] <- k
        yates[it,3] <- j
        yates[it,4] <- i
        yates[it,5] <- yates[it,5]+pred[1,1]
        
      }
    }
  }
}
for(it in 1:16){
  yates[it,5] <- yates[it,5]/4
}

modData <- data[1:250,];
yates2 <- data.frame(A=1:16,B=1:16,C=1:16,D=1:16,answer=1:16,x1=1:16,x2=1:16,x3=1:16,x4=1:16,effect=1:16)
it=0
for(i in 0:1){
  for(j in 0:1){
    for(k in 0:1){
      for( t in 0:1){
        it=it+1
        D = max(data["F3"])
        C = max(data["F4"])
        B = max(data["F2"])
        A = max(data["F1"])
        if(i==0) D = min(data["F3"])
        if(j==0) C = min(data["F4"])
        if(k==0) B = min(data["F2"])
        if(t==0) A = min(data["F1"])
        
        aux = data.frame(F3=D,
                         F4=C,
                         F2=B,
                         F1=A)
        
        pred <- predict(LM, aux, interval="confidence")
        
        yates[it,1] <- t
        yates[it,2] <- k
        yates[it,3] <- j
        yates[it,4] <- i
        yates[it,5] <- yates[it,5]+pred[1,1]
        
      }
    }
  }
}

for( j in 6:10){
  for( i in 1:16){
    if(i<=8){
      yates[i,j] = round(yates[2*i,j-1] + yates[2*i-1,j-1], digits=4)
    } else {
      yates[i,j] = round(yates[2*(i-8),j-1] - yates[2*(i-8)-1,j-1], digits=4)
    }
    
    if(j==10){
      if(i==1){
        yates[i,j] = yates[i,j-1]/16
      } else {
        yates[i,j] = yates[i,j-1]/8
      }
    }
  }
}

lmtest::dwtest(LM, alternative = "two.sided")
shapiro.test(residuals(LM))
lmtest::bptest(LM)




