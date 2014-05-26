// calculate smooth alpha1 field
fvScalarMatrix alphaSEqn
(
    fvm::Sp(scalar(1),alphaS) - fvm::laplacian(DAlpha,alphaS) == alpha1
);
alphaSEqn.solve();

// calculate curvature from smooth alpha field
surfaceVectorField gradAlphaSf = fvc::interpolate(fvc::grad(alphaS));
surfaceVectorField nHatf = gradAlphaSf / (mag(gradAlphaSf) + interface.deltaN());
volScalarField sigmaK
(
    IOobject
    (
        "sigmaK",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    -interface.sigma() * fvc::div(nHatf & mesh.Sf())
);

// additional smoothing of calculated curvature (if smoothingLoops > 0)
label smoothingLoops = 0;
for (int loop = 0; loop < smoothingLoops; loop++)
{
    surfaceScalarField sigmaKf = fvc::interpolate(sigmaK);
    forAll(sigmaK, cellI)
    {
        cell faces = mesh.cells()[cellI];
        scalar value = 0.0;
        label count = 0;
        forAll(faces, i)
        {
            label faceI = faces[i];
            if (faceI < mesh.nInternalFaces())
            {
                value += sigmaKf[faceI]; 
                count++;
            }
            else
            {
                label patchI = mesh.boundaryMesh().whichPatch(faceI);
                label faceILocal = faceI-mesh.boundaryMesh()[patchI].start();
                value += sigmaKf.boundaryField()[patchI][faceILocal];
                count++;
            }
        }
        scalar newValue = 0.5*(sigmaK[cellI]+value/count);
        sigmaK[cellI] = newValue;
    }
}

