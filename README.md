# faust2svgplot
## tool to create a svg of plotted values.

### usage
```
./faust2svgplot <myamazingdsp.dsp> <your compilation parameters>

./myamazingdsp <-n (nb_samples)[default:4096]> <-bs (buffer_size)[default:1]> <-r (sample_rate)[default:44100]>
```

### basic example 
dsp code :
```
import("stdfaust.lib");
process = 
(
    (
        os.osc(100),
        os.square(100)*0.15,
        os.triangle(400)*0.15
    )
);
```
![example](/example.png)



###  the probe metadata 
You can use the probe metadata in bargraph in order to plot at a different node of the dsp program.
dsp code example :
```cpp
import("stdfaust.lib");
process = 
(
    (
        ((os.osc(100):vbargraph("[probe:1]label1",0,1)),
        (os.square(100)*0.15:vbargraph("[probe:2]label2",0,1)),
        (os.triangle(400)*0.15:vbargraph("[probe:3]label3",0,1)))
        :>_
    )
);
```
![probe example](/probeexample.png)

### notes
* in usage, insert in <myamazingdsp.dsp> your favourite faust dsp file
* svgplot.cpp is a faust architecture file
* plot.h is the signalsmith lib file
* heatmap.h is not used for the moment

### to do
* clean everything
* for the moment the style and the colors are hard written in plot.h

