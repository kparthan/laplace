set terminal postscript eps enhanced
#set linetype 1 lc rgb "blue"
#set style fill solid 0.25 border lt 1
set style boxplot outliers pointtype 7
set style data boxplot
set boxwidth  0.05
set pointsize 0.5
unset key
set border 2
set xtics nomirror
set ytics nomirror
set xlabel "Number of samples"
set ylabel "Scale parameter"
#set grid ytics
set xrange [0:*]
set yrange [*:4.521]
set xtics ("3" 1) scale 0.0
set output "results/plots/boxplot_laplace_ml_scale_1_iter_1000.eps"
plot "results/data/laplace_ml_scale_1_iter_1000" using (1):1 lt 1 lc rgb "blue" pointtype 7, 1 with lines lt 4, "results/data/laplace_ml_scale_1_iter_1000_range" using 1:3 with lines lt 4 lc rgb "red"
set output "results/plots/boxplot_laplace_mml_scale_1_iter_1000.eps"
plot "results/data/laplace_mml_scale_1_iter_1000" using (1):1 lt 1 lc rgb "blue" pointtype 7, 1 with lines lt 4, "results/data/laplace_mml_scale_1_iter_1000_range" using 1:3 with lines lt 4 lc rgb "red"
