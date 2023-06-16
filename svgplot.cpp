/************************************************************************
 IMPORTANT NOTE : this file contains two clearly delimited sections :
 the ARCHITECTURE section (in two parts) and the USER section. Each section
 is governed by its own copyright and license. Please check individually
 each section for license and copyright information.
 *************************************************************************/

/******************* BEGIN svgplot.cpp ****************/

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2019 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 
 ************************************************************************
 ************************************************************************/


#include <iostream>
#include <string>

#include "faust/audio/channels.h"
#include "faust/dsp/dsp.h"
#include "faust/gui/console.h"
#include "faust/gui/DecoratorUI.h"
#ifdef FIXED_POINT
#include "faust/dsp/fixed-point.h"
#endif
#include "faust/misc.h"

#ifdef SOUNDFILE
#include "faust/gui/SoundUI.h"
#endif


#include "plot.h"
#include "heatmap.h"

using namespace std;

// A class to display Bargraph values
struct DisplayUI : public GenericUI {
    
    map<string, FAUSTFLOAT*> fTable;
    
    // -- passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        fTable[label] = zone;
    }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        fTable[label] = zone;
    }
    
    void displayHeaders()
    {
        int c = 0;
        if (fTable.size() > 0)
            printf(",\t");
        for (const auto& it : fTable) {
            if (c > 0)
                printf(",\t");
            printf("bargraph %d", c + 1);
            c++;
        }
    }
    
    void display()
    {
        int c = 0;
        if (fTable.size() > 0)
            printf(",\t");
        for (const auto& it : fTable) {
            if (c > 0)
                printf(",\t");
            cout << *it.second;
            c++;
        }
    }
    
};

/******************************************************************************
 *******************************************************************************
 
 VECTOR INTRINSICS
 
 *******************************************************************************
 *******************************************************************************/

<<includeIntrinsic>>

/********************END ARCHITECTURE SECTION (part 1/2)****************/

/**************************BEGIN USER SECTION **************************/

<<includeclass>>

/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/



 
dsp* DSP;

std::list<GUI*> GUI::fGuiList;
ztimedmap GUI::gTimedZoneMap;

//#define kFrames 512
//
//signalsmith::plot::PlotStyle customStyle();
//
signalsmith::plot::Plot2D plot;


//Tried to configure style but don't know how it works, I hard configure the plot.h fo the moment
//
//signalsmith::plot::PlotStyle customStyle(){
//signalsmith::plot::PlotStyle style;
//style.lineWidth = 0.1;
//style.valueSize = 1;
//style.labelSize=2;
//style.fontAspectRatio = 0.1;
//return style;
//};
//
//
int main(int argc, char* argv[])
{
    dsp* DSP = new mydsp();
    
    FAUSTFLOAT nb_samples,sample_rate, buffer_size, start_at_sample; 
    CMDUI* interface = new CMDUI(argc, argv);
    DSP->buildUserInterface(interface);
    
    interface->addOption("-n", &nb_samples, 4096.0, 0.0, 100000000.0);
    interface->addOption("-r", &sample_rate, 44100.0, 0.0, 192000.0);
    //maybe for future addons
    //interface->addOption("-bs", &buffer_size, kFrames, 0.0, kFrames * 16);
    //interface->addOption("-s", &start_at_sample, 0, 0.0, 100000000.0);

    
    if (DSP->getNumInputs() > 0) {
        cerr << "no inputs allowed " << endl;
        exit(1);
    }

    // SR has to be read before DSP init
    interface->process_one_init("-r");
    
    // init signal processor and the user interface values
    DSP->init(sample_rate);
    
    // modify the UI values according to the command line options, after init
    interface->process_init();
    
#ifdef SOUNDFILE
    SoundUI soundinterface;
    DSP->buildUserInterface(&soundinterface);
#endif

    DisplayUI disp;
    DSP->buildUserInterface(&disp);
    
    //creating inputs & outputs buffers

    FAUSTFLOAT** DSP_inputs = new FAUSTFLOAT*[DSP->getNumInputs()];
        for (int i = 0; i<DSP->getNumInputs(); i++){
                DSP_inputs[i] = new FAUSTFLOAT[int(nb_samples)];
                memset(DSP_inputs[i],0,sizeof(FAUSTFLOAT) *nb_samples);
                }

    FAUSTFLOAT** DSP_outputs = new FAUSTFLOAT*[DSP->getNumOutputs()];
        for (int i = 0; i<DSP->getNumOutputs(); i++){
                DSP_outputs[i] = new FAUSTFLOAT[int(nb_samples)];
                memset(DSP_outputs[i],0,sizeof(FAUSTFLOAT) *nb_samples);
                }
    
    //compute into inputs and outputs
    DSP->compute(nb_samples, DSP_inputs, DSP_outputs);

    //init position for max n min
    FAUSTFLOAT max = DSP_outputs[0][0]; 
    FAUSTFLOAT min =  DSP_outputs[0][0]; 
    //go through the outputs buffer
    for (int chan=0; chan< DSP->getNumOutputs(); ++chan) 
        {
        //reset for straight line and no dotted line
        plot.styleCounter.dash = 0;
        //create a line for the actual channel
        auto &line = plot.line();
        //fixed Index colour position similar to channel
        line.styleIndex.colour = chan; 
        for (int frame=0; frame < nb_samples; ++frame) 
            { 
                std::cout << "frame: "<< frame;
                FAUSTFLOAT* sub_outputs = DSP_outputs[chan];
                std::cout << " | Channel " << chan+1 << " :" << sub_outputs[frame] << "\t";
                std::cout << std::endl;
                //write points in line
                line.add(frame, sub_outputs[frame]);
              //check for max and min in order to create proportional axes
              if (max <= sub_outputs[frame]) {
              max = sub_outputs[frame];
              }
              if (min >= sub_outputs[frame]) {
              min = sub_outputs[frame];
              }  
            }
        //add label name
        line.label("Channel "+std::to_string(chan));
        }
 
    plot.x.linear(0,nb_samples).major(0).minor(nb_samples).label("Samples");
	plot.y.minors(min, max).label("Values");
    plot.y.majors(0);

    //auto style = plot.defaultStyle();
    //style.fontAspectRatio = 0.5;
    
    ////Logo Faust not working because not proportional to the axes, i tried but it's maybe a useless feature 
    //plot.image({0, nb_samples/8,min-2*min,max-max/2}, "https://faust.grame.fr/community/logos/img/LOGO_FAUST_SIMPLE_BLEU.png");

    plot.write("mydsp.svg");
    delete[] DSP_inputs;
    delete[] DSP_outputs;
    delete DSP;
    return 0;
}

/******************** END svgplot.cpp ****************/

