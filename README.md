evapVOFHardt
============

This git repository was created Bruno Santos (wyldckat@github), based on the original files that are available here: http://www.ttd.tu-darmstadt.de/forschung_1/openfoamdatenbank/evapvofhard.en.jsp - Quoting from that page:

    Solver for the simulation of two phase flows with phase change (without contact line evaporation) based on Hardt/Wondra (Journal of Computational Physics, 2008).
    In case of further questions please contact M.Sc. Stefan Batzdorf.
    
The original source code was developped for OpenFOAM 2.1.0 and this repository aims to extend functionality to other OpenFOAM versions and variants.

It has been created after finding this thread: http://www.cfd-online.com/Forums/openfoam-programming-development/135891-problem-compiling-new-solver.html


Documentation
=============

Have a look into the files `Documentation_evapVOF.pdf` and `example.mpg`.


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

These instructions are for OpenFOAM 2.1.0:

For getting and building from git:
```
git clone git://github.com/wyldckat/evapVOFHardt.git
cd evapVOFHardt
git checkout master
wmake
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/evapVOFHardt/archive/master.zip" -O evapVOFHardt.zip
cd evapVOFHardt-master
wmake
```
