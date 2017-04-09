library(doBy)

dat <- read.csv("luminosity_readings.csv", stringsAsFactors = FALSE)

# subsetting only the observations between 6 and 8 am
dat$time_simple <- substr(dat$time,12,15)
keep = c("06:0","06:1","06:2","06:3","06:4","06:5","07:0","07:1","07:2","07:3","07:4","07:5")
six_to_eight <- subset(dat, time_simple %in% keep)
six_to_eight$date <- as.POSIXct(six_to_eight$time)
six_to_eight$id <- paste0(six_to_eight$date,"_",six_to_eight$time_simple)

# making a set with the minimum and maximum
minimum <- summaryBy(luminosity ~ id + time_simple, FUN=min, six_to_eight)
maximum <- summaryBy(luminosity ~ id, FUN=max, six_to_eight)
combined <- merge(minimum, maximum, by="id")
combined$increase <- combined$luminosity.max-combined$luminosity.min
combined$time_simple <- as.factor(combined$time_simple)
combined$time_simple <- paste0(combined$time_simple,"0")
# boxplot the increase every 10 minutes
boxplot(combined$increase ~ combined$time_simple)

# adding the increase in the dataset with all the values between 6 and 8 am
change <- diff(six_to_eight$luminosity)
change <- c(0, change)
six_to_eight$change <- change

# adding a column with the minutes
six_to_eight$minute <- substr(six_to_eight$time,12,16)

# make a dataset with a column for each day
day1 <- subset(six_to_eight, date == "2017-04-03")
day1 <- summaryBy(luminosity ~ minute + time_simple, FUN=min, data=day1)
colnames(day1)[3] <- "day1"
day2 <- subset(six_to_eight, date == "2017-04-04")
day2 <- summaryBy(luminosity ~ minute, FUN=min, data=day2)
names(day2) <- c("minute", "day2")
day3 <- subset(six_to_eight, date == "2017-04-05")
day3 <- summaryBy(luminosity ~ minute, FUN=min, data=day3)
names(day3) <- c("minute", "day3")
day4 <- subset(six_to_eight, date == "2017-04-06")
day4 <- summaryBy(luminosity ~ minute, FUN=min, data=day4)
names(day4) <- c("minute", "day4")
day5 <- subset(six_to_eight, date == "2017-04-07")
day5 <- summaryBy(luminosity ~ minute, FUN=min, data=day5)
names(day5) <- c("minute", "day5")
day6 <- subset(six_to_eight, date == "2017-04-08")
day6 <- summaryBy(luminosity ~ minute, FUN=min, data=day6)
names(day6) <- c("minute", "day6")
days <- merge(day1, day2, by="minute")
days <- merge(days, day3, by="minute")
days <- merge(days, day4, by="minute")
days <- merge(days, day5, by="minute")
days <- merge(days, day6, by="minute")
days$time_simple <- paste0(days$time_simple,"0")

# add columns with the trigger values so we can plot them
days$line <- 400
days$line2 <- 260

# plot all the lines together
plot(days$day1, type="l",col="red", ylim = c(0,1026), main="Luminosity Different Days (06:00 - 08:00 am)", ylab="Luminosity", xlab="Minutes", lwd=2, lty=1)
lines(days$day2,col="green", lwd=2, lty=1)
lines(days$day3,col="darkgreen", lwd=2, lty=1)
lines(days$day4,col="orange", lwd=2, lty=1)
lines(days$day5,col="cyan", lwd=2, lty=1)
lines(days$day6,col="yellow", lwd=2, lty=1)
lines(days$line,col="black", lwd=1, lty=2)
lines(days$line2,col="black", lwd=1, lty=2)

