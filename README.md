# faust2svgplot
## tool to create a svg of plotted values.

### usage
```
./faust2svgplot <myamazingdsp.dsp> <your compilation parameters>

./myamazingdsp <-n (nb_samples)[default:4096]> <-r (sample_rate)[default:44100]>
```

### notes
* in usage, insert in <myamazingdsp.dsp> your favourite faust dsp file
* svgplot.cpp is a faust architecture file
* plot.h is the signalsmith lib file
* heatmap.h is not used for the moment

### to do
* clean everything
* for the moment the style and the colors are hard written in plot.h
* more & more
