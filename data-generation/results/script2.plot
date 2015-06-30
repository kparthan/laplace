set terminal postscript eps enhanced
unset key
set xlabel "Iterations\n"
set ylabel "Message length (in bits)\n"

set xtics font "Times-Roman, 22";
set ytics font "Times-Roman, 22";
set xlabel font "Times-Roman, 30";
set ylabel font "Times-Roman, 30";
set key font ",25";
set key spacing 1.5;

set xrange [0:*]
set yrange [1160:1300]

set output "results/plots/msglens_normal_scale_1_iter_100.eps"
plot "results/data/statistics_normal_n_100_mean_0_scale_1_iter_100" using 0:5 with linespoints lc rgb "red" title "Gaussian", \
     "results/data/statistics_normal_n_100_mean_0_scale_1_iter_100" using 0:10 with linespoints lc rgb "dark-green" title "Laplace"

set key at 95,1180
set output "results/plots/msglens_laplace_scale_1_iter_100.eps"
plot "results/data/statistics_laplace_n_100_mean_0_scale_1_iter_100" using 0:5 with linespoints lc rgb "red" title "Gaussian", \
     "results/data/statistics_laplace_n_100_mean_0_scale_1_iter_100" using 0:10 with linespoints lc rgb "dark-green" title "Laplace"
