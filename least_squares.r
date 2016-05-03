rm(list=ls())

data <- read.csv('./pingpong_out.txt')
data
summary(data)

x <- data[,1]
y <- data[,2]

line <- lsfit(x,y,intercept=TRUE)
line
latency <- coef(line)[1]
latency
slope <- coef(line)[2]
slope
bandwidth <- 1/slope
bandwidth

plot(
	x,y,
	xlab="bytes", ylab="seconds")
abline(line, col="red")
