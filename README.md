evapVOFHardt
============

This git repository was created Bruno Santos (wyldckat@github working at [blueCAPE Lda](http://www.bluecape.com.pt)), based on the original files that are available here: http://www.ttd.tu-darmstadt.de/forschung_1/openfoamdatenbank/evapvofhard.en.jsp - Quoting from that page:

    Solver for the simulation of two phase flows with phase change (without contact line evaporation) based on Hardt/Wondra (Journal of Computational Physics, 2008).
    In case of further questions please contact M.Sc. Stefan Batzdorf.
    
The original source code was developped for OpenFOAM 2.1.0 and this repository aims to extend functionality to other OpenFOAM versions and variants.

It has been created after finding this thread: http://www.cfd-online.com/Forums/openfoam-programming-development/135891-problem-compiling-new-solver.html

**Please keep in mind that this repository currently only focuses on making the solvers build with each OpenFOAM version. The results from each version have not yet been compared; and there are updates that are present in OpenFOAM's own code that have not been extended to this code.**


Documentation
=============

Have a look into the files `Documentation_evapVOF.pdf` and `example.mpg`.

Changes made that make it work differently from the original source code:

 * The utility `initField` has been renamed to `initFieldVOFHardt`.

 * Both the utility and the solver are now made available at the user's own applications folder, which means that the it's no longer necessary to run with the full path to them.

 * Added the `Allrun`, `Allclean`, `Allwmake` and `Allwclean` scripts for convenience. For example, to run the example case, simply run:
   ```
   ./Allrun
   ```


References
==========

 * S. Hardt, F. Wondra, 2008. Evaporation model for interfacial flows based on a continuum-field representation of source terms, Journal of Computational Physics, vol. 227, pp. 5871-5895.

 * C. Kunkelmann, P Stephan, 2009. CFD simulation of boiling flows using the volume-of-fluid method within OpenFOAM, Numerical Heat Transfer A, vol. 56, pp. 631-646.


Disclaimer and License
======================

Original Authors:
    Christian Kunkelmann
    Stefan Herbert
    Technische Universität Darmstadt
    Institute for Technical Thermodynamics

Original files came from here: http://www.ttd.tu-darmstadt.de/forschung_1/openfoamdatenbank/evapvofhard.en.jsp

This offering is not approved or endorsed by OpenCFD Limited, the producer of the OpenFOAM software and owner of the OPENFOAM® and OpenCFD® trade marks.

This is bound to the same license as OpenFOAM, namely GNU Public License v3. Quoting from OpenFOAM's license statement:

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM. If not, see <http://www.gnu.org/licenses/>.



How to get+build evapVOFHardt from this repository
==================================================

OpenFOAM 2.1.0, 2.1.1 and 2.1.x
-------------------------------

For getting and building from git:
```
git clone git://github.com/wyldckat/evapVOFHardt.git
cd evapVOFHardt
git checkout master
./Allwmake
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/evapVOFHardt/archive/master.zip" -O evapVOFHardt.zip
cd evapVOFHardt-master
chmod +x Allw* example/All*
./Allwmake
```


OpenFOAM 2.2.x
-------------------------------

For getting and building from git:
```
git clone git://github.com/wyldckat/evapVOFHardt.git
cd evapVOFHardt
git checkout of22x
./Allwmake
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/evapVOFHardt/archive/of22x.zip" -O evapVOFHardt.zip
cd evapVOFHardt-of22x
chmod +x Allw* example/All*
./Allwmake
```


OpenFOAM 2.3.x
-------------------------------

For getting and building from git:
```
git clone git://github.com/wyldckat/evapVOFHardt.git
cd evapVOFHardt
git checkout of23x
./Allwmake
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/evapVOFHardt/archive/of23x.zip" -O evapVOFHardt.zip
cd evapVOFHardt-of23x
chmod +x Allw* example/All*
./Allwmake
```
