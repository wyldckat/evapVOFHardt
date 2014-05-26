    Info<< "Reading field p_rgh\n" << endl;
    volScalarField p_rgh
    (
        IOobject
        (
            "p_rgh",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field alpha1\n" << endl;
    volScalarField alpha1
    (
        IOobject
        (
            "alpha1",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    volScalarField alphaS
    (
        IOobject
        (
            "alphaS",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field T\n" << endl;
    volScalarField T
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

#   include "createPhi.H"



    //- Create fields needed for phase change model
    Info<< "Reading field psi0\n" << endl;
    volScalarField psi0
    (
        IOobject
        (
            "psi0",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    volScalarField psi
    (
        IOobject
        (
            "psi",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        psi0
    );

	volScalarField rhoSource
	(
		IOobject
		(
			"rhoSource",
			runTime.timeName(),
			mesh,
			IOobject::NO_READ,
			IOobject::AUTO_WRITE
		),
		mesh,
		dimensionedScalar("rhoSource", dimensionSet(1,-3,-1,0,0,0,0), 0.0)
	);

	volScalarField hESource
	(
		IOobject
		(
			"hESource",
			runTime.timeName(),
			mesh
		),
		mesh,
		dimensionedScalar("hESource", dimensionSet(1,-1,-3,-1,0,0,0), 0.0)
	);

	volScalarField hCSource
	(
		IOobject
		(
			"hCSource",
			runTime.timeName(),
			mesh
		),
		mesh,
		dimensionedScalar("hCSource", dimensionSet(1,-1,-3,-1,0,0,0), 0.0)
	);

	volScalarField alphaSourceSu
	(
		IOobject
		(
			"alphaSourceSu",
			runTime.timeName(),
			mesh
		),
		mesh,
		dimensionedScalar("alphaSourceSu", dimensionSet(0,0,-1,0,0,0,0), 0.0)
	);

	volScalarField alphaSourceSp
	(
		IOobject
		(
			"alphaSourceSp",
			runTime.timeName(),
			mesh
		),
		mesh,
		dimensionedScalar("alphaSourceSp", dimensionSet(0,0,-1,0,0,0,0), 0.0)
	);

	dimensionedScalar mEvap ("mEvap", dimensionSet(1,0,-1,0,0,0,0), 0.0);


    //- Read transport properties of both phases
    Info<< "Reading transportProperties\n" << endl;
    twoPhaseMixture twoPhaseProperties(U, phi, "alpha1");
    
    const dimensionedScalar& rho1 = twoPhaseProperties.rho1();
    const dimensionedScalar& rho2 = twoPhaseProperties.rho2();

    IOdictionary transportProperties
    (
        IOobject
        (
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

	dictionary phase1 = transportProperties.subDict("phase1");
	dictionary phase2 = transportProperties.subDict("phase2");
	dimensionedScalar k1 = phase1.lookup("k");
	dimensionedScalar cp1 = phase1.lookup("cp");
	dimensionedScalar k2 = phase2.lookup("k");
	dimensionedScalar cp2 = phase2.lookup("cp");
	dimensionedScalar Rph = transportProperties.lookup("Rph");
	dimensionedScalar Tsat = transportProperties.lookup("Tsat");
	dimensionedScalar hEvap = transportProperties.lookup("hEvap");
	dimensionedScalar DPsi = transportProperties.lookup("DPsi");
	dimensionedScalar DAlpha = transportProperties.lookup("DAlpha");


    // Need to store rho for ddt(rho, U)
    volScalarField rho
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT
        ),
        alpha1*rho1 + (scalar(1) - alpha1)*rho2,
        alpha1.boundaryField().types()
    );
    rho.oldTime();


    //- Mass flux
    //  Initialisation does not matter because rhoPhi is reset after the
    //  alpha solution before it is used in the U equation.
    surfaceScalarField rhoPhi
    (
        IOobject
        (
            "rho*phi",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        rho1*phi
    );

    //- Construct interface from alpha1 distribution
    interfaceProperties interface(alpha1, U, twoPhaseProperties);


    //- Construct incompressible turbulence model
    autoPtr<incompressible::turbulenceModel> turbulence
    (
        incompressible::turbulenceModel::New(U, phi, twoPhaseProperties)
    );


    Info << "Calculation field g.h\n" << endl;
    volScalarField gh ("gh", g & mesh.C());
    surfaceScalarField ghf ("ghf", g & mesh.Cf());

    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        p_rgh + rho*gh
    );

    label pRefCell = 0;
    scalar pRefValue = 0.0;
    setRefCell
    (
        p,
        p_rgh,
        mesh.solutionDict().subDict("PISO"),
        pRefCell,
        pRefValue
    );

    if (p_rgh.needReference())
    {
        p += dimensionedScalar
        (
            "p",
            p.dimensions(),
            pRefValue - getRefCellValue(p, pRefCell)
        );
        p_rgh = p - rho*gh;
    }
