/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2007 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Application
    evapVOFHardt

Description
    Solver for 2 incompressible, immiscible, evaporating fluids using a VOF 
    (volume of fluid) phase-fraction based interface capturing approach, 
    with optional mesh motion and mesh topology changes including adaptive 
    re-meshing.

Authors
    Christian Kunkelmann
    Stefan Herbert
    Technische Universität Darmstadt
    Institute for Technical Thermodynamics

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "MULES.H"
#include "dynamicFvMesh.H"
#include "dynamicRefineFvMesh.H"
#include "subCycle.H"
#include "interfaceProperties.H"
#include "incompressibleTwoPhaseMixture.H"
#include "probes.H"
#include "alphaContactAngleFvPatchScalarField.H"
#include "turbulenceModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

#   include "setRootCase.H"
#   include "createTime.H"
#   include "createDynamicFvMesh.H"
#   include "readGravitationalAcceleration.H"
#   include "readPISOControls.H"
#   include "initContinuityErrs.H"
#   include "createFields.H"
#   include "readTimeControls.H"
#   include "correctPhi.H"
#   include "CourantNo.H"
#   include "setInitialDeltaT.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
#       include "readControls.H"
#       include "alphaCourantNo.H"
#       include "CourantNo.H"
#       include "setDeltaT.H"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        scalar timeBeforeMeshUpdate = runTime.elapsedCpuTime();

        // Do any mesh changes
        mesh.update();

        if (mesh.changing())
        {
            Info<< "Execution time for mesh.update() = "
                << runTime.elapsedCpuTime() - timeBeforeMeshUpdate
                << " s" << endl;

            gh = g & mesh.C();
            ghf = g & mesh.Cf();
        }

        if (mesh.changing() && correctPhi)
        {
            //phi = fvc::interpolate(U)&mesh.Sf();   <-- Need to test if still needed!
            #include "correctPhi.H"
        }

        // Make the fluxes relative to the mesh motion
        fvc::makeRelative(phi, U);

        if (mesh.changing() && checkMeshCourantNo)
        {
            #include "meshCourantNo.H"
        }
        
        twoPhaseProperties.correct();

		// source terms for gamma equation (due to mass sources and resulting divergence in velocity field)
        alphaSourceSu = fvc::div(phi)*alpha1;
        alphaSourceSp = fvc::div(phi)*0.0;
        
#       include "alphaEqnSubCycle.H"

#		include "surfaceTension.H"

        for (int loop = 0; loop < 2; loop++)
        {
#		    include "calcSourceTerms.H"
        	mEvap = intPsi0;
            if (loop == 0)
            {
#		        include "TEqn.H"
            }
        }

		Info << "Evaporation rate: " << mEvap.value() << " kg/s at t = " << runTime.timeName() << " s" << endl;

#       include "UEqn.H"

        // --- PISO loop
        for (int corr=0; corr<nCorr; corr++)
        {
#           include "pEqn.H"
        }

        turbulence->correct();

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return(0);
}


// ************************************************************************* //
