# faust2svgplot
## tool for creating a svg of plotted values.

### usage
```
faust -a svgplot.cpp -cn mydsp <mydsp.dsp> -o mydsp.cpp

c++ -std=c++11 -g mydsp.cpp `pkg-config --cflags --libs jack gtk+-2.0` -o faust2svgplot

./faust2svgplot <-n (nb_samples)[default:4096]> <-r (sample_rate)[default:44100]>
```

### notes
* in usage, insert in <mydsp.dsp> your favourite faust dsp file
* svgplot.cpp is a faust architecture file
* plot.h is the signalsmith lib file

### to do
* clean everything
* for the moment the style and the colors are hard written in plot.h
* more & more
