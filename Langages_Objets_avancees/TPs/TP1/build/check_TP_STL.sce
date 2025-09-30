// check_TP_STL checks TP STL results
//   checks provided mean, std and median values
function checked = check_TP_STL( values, minValue, maxValue, meanValue, stdValue, medianValue )
	meanComputed = mean(values);
	minComputed = min(values);
	maxComputed = max(values);
	stdComputed = std(values, 1);
	medianComputed = median(values);
	epsilon = 1e-3;
	checked = true;
	
	fprintf('min value    = %f --> ', minComputed);
	minDiff = minComputed - minValue;
	if minDiff > 0
		fprintf('Ok > %f\n', minValue);
	else
		fprintf('Ko by %f\n', minDiff);
		checked = false;
	end
	
	fprintf('max value    = %f --> ', maxComputed);
	maxDiff = maxValue - maxComputed;
	if maxDiff > 0
		fprintf('Ok < %f\n', maxValue);
	else
		fprintf('Ko by %f\n', maxDiff);
		checked = false;
	end
	
	fprintf('mean value   = %f --> ', meanValue);
	meanDiff = abs(meanComputed - meanValue);
	if meanDiff < epsilon
		fprintf('Ok\n');
	else
		fprintf('Ko by %f\n', meanDiff);
		checked = false;
	end
	
	fprintf('std value    = %f --> ', stdValue);
	stdDiff = abs(stdComputed - stdValue);
	if stdDiff < epsilon
		fprintf('Ok\n');
	else
		fprintf('Ko by %f\n', stdDiff);
		checked = false;
	end
	
	fprintf('median value = %f --> ', medianValue);
	medianDiff = abs(medianComputed - medianValue);
	if medianDiff < epsilon
		fprintf('Ok\n');
	else
		fprintf('Ko by %f\n', medianDiff);
		checked = false;
	end
end
