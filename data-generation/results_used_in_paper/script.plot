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
set xlabel "Sample size" font ",20" 
set ylabel "Estimated scale parameter" font ",25"
#set grid ytics
set xrange [0:*]
set yrange [*:5.164]
set xtics ("2" 1, "3" 2, "4" 3, "5" 4, "10" 5, "15" 6, "25" 7, "50" 8, "100" 9, "1000" 10) scale 0.0
set output "results/plots/boxplot_laplace_ml_scale_1_iter_100.eps"
plot "results/data/laplace_ml_scale_1_iter_100" using (1):1 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_ml_scale_1_iter_100" using (2):2 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_ml_scale_1_iter_100" using (3):3 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_ml_scale_1_iter_100" using (4):4 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_ml_scale_1_iter_100" using (5):5 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_ml_scale_1_iter_100" using (6):6 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_ml_scale_1_iter_100" using (7):7 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_ml_scale_1_iter_100" using (8):8 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_ml_scale_1_iter_100" using (9):9 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_ml_scale_1_iter_100" using (10):10 lt 1 lc rgb "blue" pointtype 7, 1 with lines lt 4, "results/data/laplace_ml_scale_1_iter_100_range" using 1:3 with lines lt 4 lc rgb "red"
set output "results/plots/boxplot_laplace_mml_scale_1_iter_100.eps"
plot "results/data/laplace_mml_scale_1_iter_100" using (1):1 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_mml_scale_1_iter_100" using (2):2 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_mml_scale_1_iter_100" using (3):3 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_mml_scale_1_iter_100" using (4):4 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_mml_scale_1_iter_100" using (5):5 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_mml_scale_1_iter_100" using (6):6 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_mml_scale_1_iter_100" using (7):7 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_mml_scale_1_iter_100" using (8):8 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_mml_scale_1_iter_100" using (9):9 lt 1 lc rgb "blue" pointtype 7, "results/data/laplace_mml_scale_1_iter_100" using (10):10 lt 1 lc rgb "blue" pointtype 7, 1 with lines lt 4, "results/data/laplace_mml_scale_1_iter_100_range" using 1:3 with lines lt 4 lc rgb "red"
