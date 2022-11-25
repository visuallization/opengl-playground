# Forest Fire Performance
Measuring the time (in ms) of the forest fire algorithm for calculating 1, 10, 100, 1000 and 10000 steps. The algorithm runs on a compute shader.

## Performance

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