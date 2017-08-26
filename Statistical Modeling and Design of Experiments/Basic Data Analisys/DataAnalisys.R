#generation of a normal distribution.
v1=rnorm(200, mean=0, sd=1)
summary(v1)

#Work with the data as a dataframe.
taula_v1=data.frame(x1=v1)

#Definition of the intervals, categories to be used.
taula_v1_cat=transform(taula_v1, cat = ifelse(x1 < -1,"-1",
                                              ifelse(x1 < -0.5,"-0.5",
                                                     ifelse(x1 < 0,"0",
                                                            ifelse(x1 < 0.5,"0.5",
                                                                   ifelse(x1 <1,"1","Inf"))))))

#Counting the amount of elements in each category "table" function.
taula_freq_v1=as.data.frame(with(taula_v1_cat, table(cat)))

Test=chisq.test(taula_freq_v1, correct=FALSE)

colnames(decathlon)[which(names(decathlon) == "1500m")] <- "X1500m"
colnames(decathlon)[which(names(decathlon) == "100m")] <- "X100m"
colnames(decathlon)[which(names(decathlon) == "110m.hurdle")] <- "X110m.hurdle"

regModels <- list()
max = 0.0
min = 100
for( i in names(decathlon)){
  for( j in names(decathlon)){
    for( t in names(decathlon)){
      for( q in names(decathlon)){
        if( i != "X1500m" && i!=j && i!=t && j!=t && j!="X1500m" && t!="X1500m" && q!="X1500m" && i!="Competition" && j!="Competition" && t!="Competition" & q!="Competition"){
        #if( i != "X1500m" && i!=j && i!=t && j!=t && j!="X1500m" && t!="X1500m"){
          # print(i)
          x <- decathlon[[i]]
          x2 <- decathlon[[j]]
          x3 <- decathlon[[t]]
          x4 <- decathlon[[q]]
          #regModels[[i]] <- lm(X1500m~x+x2+x3, data=decathlon)
          
          RegModel <- lm(X1500m~x+x2+x3+x4, data=decathlon)
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


RegModel <- lm(X1500m~X400m, data=decathlon)
summary(RegModel)

print(mean(decathlon[["X1500m"]]))

plot(RegModel)

plot(decathlon[["X400m"]],decathlon[["X1500m"]], abline(RegModel))

new <- data.frame(X400m=48)
predict(RegModel, newdata=new, interval="prediction")
predict(RegModel, newdata=new, interval="confidence")

AnovaModel.1 <- aov(Block.size.median ~ Drift, data=geomorphology)
summary(AnovaModel.1)
plot(Block.size.median~Drift, data=geomorphology, id.method="y")
lmtest::bptest(AnovaModel.1)
lmtest::dwtest(AnovaModel.1, alternative = "two.sided")

AnovaModel.1 <- aov(Wind.effect ~ Drift, data=geomorphology)
summary(AnovaModel.1)
plot(Wind.effect~Drift, data=geomorphology, id.method="y")
lmtest::bptest(AnovaModel.1)
lmtest::dwtest(AnovaModel.1, alternative = "two.sided")

plot(AnovaModel.1)

X1500m <- decathlon[["X1500m"]]-mean(decathlon[["X1500m"]])
data = data.frame(X1500m)

for( i in names(decathlon)){
  if( i != "X1500m" && i!="Competition"){
    x <- decathlon[[i]]
    mx <- mean(x)
    data[i] <- x
  }
}

data <- data.matrix(data)

#Computing the covariance matrix (10 because we have 10 elements on the data).
covData<-(1/1000)*t(data2)%*%data2

#Eigenvectors of the ovariance matrix
pcas<-eigen(covData)
pcas$vectors

#p2 <- prcomp( ~ X1500m+Pole.vault+Points+Discus, data=data, scale = TRUE)
p2 <- prcomp(data, scale = TRUE)
print(p2)
plot(p2)
biplot(p2)

p1 <- princomp( data, cor = TRUE)
summary(p1)
loadings(p1)
plot(p1)
biplot(p1)
p1$scores

x100m = rnorm(20, mean=11, sd=0.15)
pole = rnorm(20, mean=4.75, sd=0.25)
points = rnorm(20, mean=8000, sd=400)
discus = rnorm(20, mean=47, sd=2)
new <- data.frame(Points=points,X100m=x100m,Discus=discus,Pole.vault=pole)

dataf = lm(X1500m~Points+X100m+Discus+Pole.vault, data=decathlon)

pr =  data.frame(1,2,3)
names(pr)<-c("fit","lwr","upr")data

for (i in 0:nrow(new)) 
{
  new3 = data.frame(Points=new[i,"Points"],
                    X100m=new[i,"X100m"],
                    Discus=new[i,"Discus"],
                    Pole.vault=new[i,"Pole.vault"])
  pr<-rbind(predict(dataf, new3, interval="confidence"), pr)
}

