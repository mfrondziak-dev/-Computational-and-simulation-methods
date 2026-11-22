set encoding utf8
set terminal svg size 1100,700 enhanced font "Arial,12"
set grid
set key top left

set output "exp_cos.svg"
set title "exp(x) cos(0.5 x^2), przedzial [0,3]"
set xlabel "x"
set ylabel "y"
plot "exp_cos.dat" using 1:2 with lines lw 2 title "funkcja", \
     "exp_cos.dat" using 1:3 with lines title "aproksymacja n=3", \
     "exp_cos.dat" using 1:4 with lines title "aproksymacja n=10", \
     "exp_cos.dat" using 1:5 with lines title "aproksymacja n=40"

set output "rational.svg"
set title "1/(12 x^2 + 1), przedzial [-2,2]"
set xlabel "x"
set ylabel "y"
plot "rational.dat" using 1:2 with lines lw 2 title "funkcja", \
     "rational.dat" using 1:3 with lines title "aproksymacja n=3", \
     "rational.dat" using 1:4 with lines title "aproksymacja n=10", \
     "rational.dat" using 1:5 with lines title "aproksymacja n=40"

set output "oscillatory.svg"
set terminal svg size 1500,700 enhanced font "Arial,12"
set title "cos(x) + 0.05 sin(25 x), przedzial [0,10]"
set xlabel "x"
set ylabel "y"
set key top right
plot "oscillatory.dat" using 1:2 with lines lw 2 title "funkcja", \
     "oscillatory.dat" using 1:3 with lines title "aproksymacja n=40", \
     "oscillatory.dat" using 1:4 with lines title "aproksymacja n=256"

unset output
