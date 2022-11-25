# Forest Fire
Implementation of the forest fire algorithm.  

The project uses my custom render framework which I started in the second semester and continue to improve upon ever since.  
The framework + project is written C++ and OpenGL. The algorithm runs on a compute shader to take advantage of parallelization.

## How does it work?

The whole logic of the forest fire algorithm is happening in a compute shader. I send 2 textures to the compute shader to write and read from. At the first draw call I will read from texture 1 and write to texture 2. This is necessary for the algorithm to work properly because it needs to get the old/unchanged state of the neighbor cells and not newly written ones.
In the next draw call I will switch textures and the compute shader will now read from texture 2 and write to texture 1. This proceedure of texture switching goes on with every frame.

The texure itself is being fed to a vertex and fragment shader, which renders the texture on a window-sized quad. The vertex and fragment shader will also use with ervery frame the switched texture, namely the one the compute shader has just written into.

## Performance

The compute shader instances 1000x1000 workgroups and 1x1 invocations per workgroup. Since the window size is 1000x1000, 
this basically means the compute shader is running a workgroup for every pixel. This configuration is running with around 400 FPS on my notebook.

One can improve this further by only instancing 100x100 workgroups and increasing to 10x10 invocations per workgroup. In other words we split the texture into batches of a certain size and run over each of this batches per workgroup. This configuration is running with around 1500 FPS on my notebook. Which is around 3 times faster than the per pixel configuration.

Following are some measurements which are measuring the time (in ms) of the forest fire algorithm for calculating 1, 10, 100, 1000 and 10000 steps.

**400 fps** (1000x1000 workgroups | 1x1 invocations per workgroup)
```
1       1.4514 ms  
10      16.5787 ms  
100     218.99 ms  
1000    2662.51 ms  
10000   23323.4 ms  
```

**1500 fps** (100x100 workgroups | 10x10 invocations per workgroup)
```
1       1.7427 ms  
10      7.4755 ms  
100     63.7799 ms  
1000    623.033 ms  
10000   6591.87 ms  
```

## Hardware Specs
The measurments were done on the following hardware:

### CPU
```
AMD Ryzen 9 5900HX with Radeon Graphics  

Basisgeschwindigkeit:	3,30 GHz  
Sockets:	1  
Kerne:	8  
Logische Prozessoren:	16  
Virtualisierung:	Aktiviert  
L1-Cache:	512 KB  
L2-Cache:	4,0 MB  
L3-Cache:	16,0 MB  
```
### GPU
```
NVIDIA GeForce RTX 3060 Laptop GPU  

Treiberversion:	31.0.15.2686  
Treiberdatum:	08.11.2022  
DirectX-Version:	12 (FL 12.1)  
Physischer Standort:	PCI-Bus 1, Gerät 0, Funktion 0  

Auslastung	0%  
Dedizierter GPU-Speicher	6,0 GB  
Gemeinsamer GPU-Speicher	15,7 GB  
GPU-Speicher	21,7 GB  
```
### RAM
```
32,0 GB  

Geschwindigkeit:	3200 MHz  
Steckplätze verwendet:	2 von 2  
Formfaktor:	SODIMM  
Für Hardware reserviert:	650 MB  
```