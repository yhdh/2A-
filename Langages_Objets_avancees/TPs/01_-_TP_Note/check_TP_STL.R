checkvalues <- function(values, minValue, maxValue, meanValue, stdValue, medianValue) 
{
	meanComputed = mean(values)
	minComputed = min(values)
	maxComputed = max(values)
	stdComputed = sqrt(mean(values^2) - meanComputed ^2)
	medianComputed = median(values)
	epsilon = 1e-3
	checked = TRUE
	
	cat("min value	= ", minComputed, " --> ")
	minDiff = minComputed - minValue
	if (minDiff > 0) 
	{
		cat("Ok > ", minValue, "\n")
	}
	else
	{
		cat("Ko by ", minDiff, "\n")
		checked = FALSE
	}
	
	cat("max value	= ", maxComputed, " --> ")
	maxDiff = maxValue - maxComputed
	if (maxDiff > 0) 
	{
		cat("Ok < ", maxValue, "\n")
	}
	else
	{
		cat("Ko by ", maxDiff, "\n")
		checked = FALSE
	}
	
	cat("mean value	= ", meanValue, " --> ")
	meanDiff = abs(meanComputed - meanValue)
	if (meanDiff < epsilon) 
	{
		cat("Ok (", meanComputed, ")\n")
	}
	else
	{
		cat("Ko by ", meanDiff, "\n")
		checked = FALSE
	}

	cat("std value	= ", stdValue, " --> ")
	stdDiff = abs(stdComputed - stdValue)
	if (stdDiff < epsilon) 
	{
		cat("Ok (", stdComputed, ")\n")
	}
	else
	{
		cat("Ko by ", stdDiff, "\n")
		checked = FALSE
	}

	cat("median value	= ", medianValue, " --> ")
	medianDiff = abs(medianComputed - medianValue)
	if (medianDiff < epsilon) 
	{
		cat("Ok (", medianComputed, ")\n")
	}
	else
	{
		cat("Ko by ", medianDiff, "\n")
		checked = FALSE
	}
}

#frame = read.csv("test.csv", header = TRUE, sep = ";")
#values = frame$Values
#print(values)
#mean_value = mean(values)
#std_value = sqrt(mean(values^2) - mean_value^2)
#sprintf("%f ± %f", mean_value, std_value)
