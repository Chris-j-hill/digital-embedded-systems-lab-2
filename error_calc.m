% for calculating the crossover error between counting edges and counting
% clock cycles


max_freq = 100000;
error1 = zeros(1,max_freq);     % measuring period

error2 = zeros(1,max_freq);     % measuring edges

time_interval = 1;
samp_freq = 100000;

for  freq = [1:max_freq]

    
    
%     error1(freq) = ((freq+0.49)/samp_freq);       
    
    edges_measured = freq*time_interval;
    
    error2(freq) = (1/((edges_measured+0.49)/time_interval));
end



plot([1:max_freq], error1)
hold on
plot([1:max_freq], error2)
hold off