set term postscript eps enhanced color
#set output "results.eps"
set xlabel "Number of threads"
set ylabel "Execution time (seconds)"
plot "../images/results" using 1:2 with linespoints title "MaximumProjectionImageFilter"\
, "../images/results" using 1:3 with linespoints title "MinimumProjectionImageFilter"\
, "../images/results" using 1:4 with linespoints title "SumProjectionImageFilter"\
, "../images/results" using 1:6 with linespoints title "MeanProjectionImageFilter"\
, "../images/results" using 1:8 with linespoints title "MedianProjectionImageFilter"\
, "../images/results" using 1:9 with linespoints title "SigmaProjectionImageFilter"\
, "../images/results" using 1:10 with linespoints title "BinaryProjectionImageFilter"\
