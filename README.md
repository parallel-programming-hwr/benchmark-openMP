# benchmark-openMP
some benchamarks with openMP in C

## compile
make to compile

## usage 
then 
```bash
./collect_data.sh <num> <outdata.txt> 
```
to run program with 1 to num threads.

You can specify reps per iteration and iterations in collect\_data.sh.

Run 
```bash
./make_plots.py <indata.txt> <outplot.png>
```
to make plot.
