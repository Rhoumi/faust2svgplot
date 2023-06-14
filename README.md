# faust2svgplot
## tool for creating a svg of plot values.

### usage
```
c++ -std=c++11 -g csvplot.cpp `pkg-config --cflags --libs jack gtk+-2.0` -o faust2svgplot

./faust2svgplot <-n (nb_samples)[default:4096]> <-r (sample_rate)[default:44100]>
```

### notes
* mydsp.cpp is for testing the arch
* plot.h & heatmap.h are the signalsmith lib files

### to do
* clean everything
* add multiple channels possibility
* solve axes label value nonsense
* more & more
