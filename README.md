# faust2svgplot
## tool for creating a svg of plotted values.

### usage
```
c++ -std=c++11 -g svgplot.cpp `pkg-config --cflags --libs jack gtk+-2.0` -o faust2svgplot

./faust2svgplot <-n (nb_samples)[default:4096]> <-r (sample_rate)[default:44100]>
```

### notes
* mydsp.cpp is for testing the arch / replace with your dsp.cpp in line 67 in svgplot.cpp
* mydsp2.cpp and mydsp9.cpp are for multichannel tests
* plot.h is the signalsmith lib file

### to do
* clean everything
* for the moment the style and the colors are hard written in plot.h
* more & more
