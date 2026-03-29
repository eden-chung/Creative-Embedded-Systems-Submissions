# Interactive Devices Project

## Materials List
- LilyGo ESP32
- Breadboard
- Wires
- Copper tape
- 3D printed enclosure
- USB-C Cable

## Documentation from start to finish
- Measure the ESP32 using a caliper, use these measurements to create an accurate enclosure box for the breadboard and ESP32.
- Using SCAD (programmatically create CAD files), enter the measurements to get a CAD file for the box and lid. This file can be found [here](<3D Printing/enclosure_box_and_lid.scad>)
- Save the CAD files as STLs using the software OpenSCAD. Save the box and the lid separately so that they can be individually moved in software such as Ultimaker for sending to the 3D printer
- 3D print the box, lid, and the 3D model of a cat, found [here](<3D Printing/cat.stl>) (credit to [Gacho Banduga from makerworld](https://makerworld.com/en/models/2206120-oiia-cat?from=search#profileId-2397570
)).
- If the size is the same, the direct STL files can be used and the above skips can be skipped. [Box STL](<3D Printing/eesp32_box.stl>) and [Lid STL](<3D Printing/eesp32_lid.stl>)

![3D prints](images/IMG_9300.png)

## Software and Firmware
- Install the [firmware](touch_sensor.cpp) onto the ESP32 by using PlatformIO in VSCode and selecting Build, then Upload.
- To run locally, the HTML file, [index.html](index.html) can be run using the following command from the same directory as the file\
`python -m http.server 8000`
- The HTML file uses three.js to render a cat spinning (3D render credit to [Zhuier from Sketchfab](https://sketchfab.com/3d-models/oiiaioooooiai-cat-30d27bf7fb224849b76e208a6eccdb36))

Demo of website
![demo](images/demo.mov)


# Troubleshooting
- Serial monitor cannot be open at same time as connecting to website. Only one thing can use the serial monitor at once
- Similarly, if having trouble using the "Upload" function on PlatformIO, ensure the device is not connecting to the web server.