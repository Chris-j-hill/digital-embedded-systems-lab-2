
% freq_range = [1:100000];
max_freq = 100000;
error1 = zeros(1,max_freq);     % measuring period

error2 = zeros(1,max_freq);     % measuring edges

time_interval = 1;

samp_freq = 1000000;

for  freq = [1:max_freq]
    
    error1(freq) = freq/samp_freq;
    error2(freq) = (time_interval*freq);
end

