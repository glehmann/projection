set term postscript eps enhanced color
#set output "results.eps"
set xlabel "Number of threads"
set ylabel "Execution time (seconds)"
plot [] [0:0.035] "../images/results" using 1:2 with linespoints title "MaximumProjectionImageFilter"\

