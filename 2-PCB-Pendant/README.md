# Creating a PBC Pendant with a 555 Timer

### Materials List
- Copper Board
- 470 Ohm Resistor
- 10 KOhm Resistor
- 100 KOhm Resistor
- 2.2 uF Capacitor
- 10 nF Capacitor
- Battery
- LED
- 555 Timer

### Designing and building the PCB
In KiCad, the schematic editor was used to create the 555 timer circuit. This file can be found [here](555timerCES.kicad_sch)

![schematic](images/circuit.png)

 Once done, the PCB editor was used to create a layout such that no wires were overlapping, as the copper board is a 1 layer board. Then, the components were connected to create the wire outline. This file can be found [here](555timerCES.kicad_sch)

![pcb](images/pcb.png)

The resulting PCB and schematic files were then used to convert into SVG files that the CNC milling machine was able to cut. 

To transfer these SVGs onto the CNC machine, an online application called easel was used.

The milling machine has to be used in 3 separate steps:
- Etching to create the wires. In this step, the machine has to cut through the copper to make wires in the board, but not too deep such that it goes all the way through. SVG file [here](simple_front.svg).
- Cutting holes in the PCB. The machine drills holes to allow the components to be put through then soldered at a later stage. SVG file [here](simple_drill.svg).
- Cutting the outline of the PCB. With the given shape, the machine cuts the outline of the entire PCB. SVG file [here](simple_outline.svg).


### Soldering
Each part was soldered to the copper board, following the layout from KiCad, making sure there were no shorts and that every component was fully connected.

The battery can finally be inserted into the battery holder, creating a flashing light pattern on the PCB.

![final_product](images/final_product.png)




