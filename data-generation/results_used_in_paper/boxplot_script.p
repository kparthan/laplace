set terminal postscript eps enhanced
#set style fill solid 0.25 border -1
set style boxplot outliers pointtype 7
set style data boxplot
set boxwidth  0.05
set pointsize 0.5
unset key
set border 2

set ytics nomirror

#set xtics ("2" 1, "3" 2, "4" 3, "5" 4, "10" 5, "15" 6, "25" 7, "50" 8, "100" 9, "1000" 10) scale 0.0
set xtics ("3" 1, "4" 2, "5" 3, "10" 4, "15" 5, "25" 6, "50" 7, "100" 8, "1000" 9) scale 0.0
set xtics font "Times-Roman, 25"
set ytics font "Times-Roman, 25"

set xlabel "Sample size\n" offset 0,-1
set ylabel "Estimated scale parameter\n"
set xlabel font "Times-Roman, 35"
set ylabel font "Times-Roman, 35"

set xrange [0:*]
set yrange [0:3.5]

set output "plots/boxplot_laplace_ml_scale_1_iter_100.eps" 
plot "data/laplace_ml_scale_1_iter_100" using (1):2 lt 1 lc rgb "blue" pointtype 7, \
     "" using (2):3 lt 1 lc rgb "blue" pointtype 7, \
     "" using (3):4 lt 1 lc rgb "blue" pointtype 7, \
     "" using (4):5 lt 1 lc rgb "blue" pointtype 7, \
     "" using (5):6 lt 1 lc rgb "blue" pointtype 7, \
     "" using (6):7 lt 1 lc rgb "blue" pointtype 7, \
     "" using (7):8 lt 1 lc rgb "blue" pointtype 7, \
     "" using (8):9 lt 1 lc rgb "blue" pointtype 7, \
     "" using (9):10 lt 1 lc rgb "blue" pointtype 7, \
     1 with lines lt 4, \
     "medians_ml" using ($0+1):1 with linespoints lt 4 lc rgb "red"

set output "plots/boxplot_laplace_mml_scale_1_iter_100.eps" 
plot "data/laplace_mml_scale_1_iter_100" using (1):2 lt 1 lc rgb "blue" pointtype 7, \
     "" using (2):3 lt 1 lc rgb "blue" pointtype 7, \
     "" using (3):4 lt 1 lc rgb "blue" pointtype 7, \
     "" using (4):5 lt 1 lc rgb "blue" pointtype 7, \
     "" using (5):6 lt 1 lc rgb "blue" pointtype 7, \
     "" using (6):7 lt 1 lc rgb "blue" pointtype 7, \
     "" using (7):8 lt 1 lc rgb "blue" pointtype 7, \
     "" using (8):9 lt 1 lc rgb "blue" pointtype 7, \
     "" using (9):10 lt 1 lc rgb "blue" pointtype 7, \
     1 with lines lt 4, \
     "medians_mml" using ($0+1):1 with linespoints lt 4 lc rgb "red"

#set output "plots/boxplot_laplace_ml_scale_1_iter_100.eps" 
#plot "data/laplace_ml_scale_1_iter_100" using (1):1 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (2):2 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (3):3 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (4):4 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (5):5 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (6):6 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (7):7 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (8):8 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (9):9 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (10):10 lt 1 lc rgb "blue" pointtype 7, \
#     1 with lines lt 4, \
#     "medians_ml" using ($0+1):1 with linespoints lt 4 lc rgb "red"
#
#
#set output "plots/boxplot_laplace_mml_scale_1_iter_100.eps" 
#plot "data/laplace_mml_scale_1_iter_100" using (1):1 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (2):2 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (3):3 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (4):4 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (5):5 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (6):6 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (7):7 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (8):8 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (9):9 lt 1 lc rgb "blue" pointtype 7, \
#     "" using (10):10 lt 1 lc rgb "blue" pointtype 7, \
#     1 with lines lt 4, \
#     "medians_mml" using ($0+1):1 with linespoints lt 4 lc rgb "red"
