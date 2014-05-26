//- alpha cutoff value for source term distribution 
//  (no source terms in cells with cutoff < alpha1 < 1-cutoff)
scalar cutoff = 1e-3;

//- Calculation of source terms according to Hardt/Wondra (JCP, 2008)
volScalarField psi0Tild = mag(fvc::grad(alpha1));

dimensionedScalar intPsi0Tild = fvc::domainIntegrate(psi0Tild);
dimensionedScalar intAlphaPsi0Tild = fvc::domainIntegrate(alpha1*psi0Tild);

dimensionedScalar N ("N", dimensionSet(0,0,0,0,0,0,0), 2.0);
if (intAlphaPsi0Tild.value() > 1e-99)
{
	N = intPsi0Tild/intAlphaPsi0Tild;
}

volScalarField jEvap = (T-Tsat)/(Rph*hEvap);

psi0 = N*jEvap*alpha1*psi0Tild;

dimensionedScalar intPsi0 = fvc::domainIntegrate(psi0);

//- Smearing of source term field
fvScalarMatrix psiEqn
(
	fvm::Sp(scalar(1),psi) - fvm::laplacian(DPsi,psi) == psi0
);

psiEqn.solve();

//- Cut cells with cutoff < alpha1 < 1-cutoff and rescale remaining source term field
dimensionedScalar intPsiLiquid ("intPsiLiquid", dimensionSet(1,0,-1,0,0,0,0), 0.0);
dimensionedScalar intPsiVapor ("intPsiLiquid", dimensionSet(1,0,-1,0,0,0,0), 0.0);

forAll(mesh.C(),iCell)
{
	if (alpha1[iCell] < cutoff)
	{
		intPsiVapor.value() += (1.0-alpha1[iCell])*psi[iCell]*mesh.V()[iCell];
	}
	else if (alpha1[iCell] > 1.0-cutoff)
	{
		intPsiLiquid.value() += alpha1[iCell]*psi[iCell]*mesh.V()[iCell];
	}
}

//- Calculate Nl and Nv
dimensionedScalar Nl ("Nl", dimensionSet(0,0,0,0,0,0,0), 2.0);
dimensionedScalar Nv ("Nv", dimensionSet(0,0,0,0,0,0,0), 2.0);

reduce(intPsiLiquid.value(),sumOp<scalar>());
reduce(intPsiVapor.value(),sumOp<scalar>());

if (intPsiLiquid.value() > 1e-99)
{
    Nl = intPsi0/intPsiLiquid;
}
if (intPsiVapor.value() > 1e-99)
{
    Nv = intPsi0/intPsiVapor;
}

        
//- Set source terms in cells with alpha1 < cutoff or alpha1 > 1-cutoff
forAll(mesh.C(),iCell)
{
	if (alpha1[iCell] < cutoff)
	{
		rhoSource[iCell] = Nv.value()*(1.0-alpha1[iCell])*psi[iCell];
	}
	else if (alpha1[iCell] > 1.0-cutoff)
	{
		rhoSource[iCell] = -Nl.value()*alpha1[iCell]*psi[iCell];
	}
	else
	{
		rhoSource[iCell] = 0.0;
	}
}

//- Evaporation source term in energy equation
hESource = -N*alpha1*psi0Tild/Rph;
