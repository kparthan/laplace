# Gnuplot script file for plotting data in file "data"

set terminal post eps
set autoscale	# scale axes automatically
set xtic auto	# set xtics automatically
set ytic auto	# set ytics automatically
set title ""
set xlabel "x\n"
set ylabel "probability density\n"
set xr [-6:6]
set yr [0:0.7]
set output "results/plots/laplace_n_100_mean_0_scale_1_iter_1.eps"
set multiplot
set xtics font "Times-Roman, 25"
set ytics font "Times-Roman, 25"
set xlabel font "Times-Roman, 35"
set ylabel font "Times-Roman, 35"
set key font ",25"
set key spacing 1.5
plot "results/data/laplace_n_100_mean_0_scale_1_iter_1" using 1:2 title 'original' with points lc rgb "red", \
"results/data/laplace_n_100_mean_0_scale_1_iter_1" using 1:3 title 'Gaussian' with points lc rgb "blue", \
"results/data/laplace_n_100_mean_0_scale_1_iter_1" using 1:4 title 'Laplace' with points lc rgb "dark-green"
