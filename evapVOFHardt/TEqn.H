volScalarField k = alpha1*k1+(scalar(1)-alpha1)*k2;
// turbulent heat transfer is to be added some time

volScalarField rhoCp = alpha1*rho1*cp1+(scalar(1)-alpha1)*rho2*cp2;
rhoCp.oldTime() = alpha1.oldTime()*rho1*cp1+(scalar(1)-alpha1.oldTime())*rho2*cp2;

surfaceScalarField alphaPhi = (rhoPhi-phi*rho2)/(rho1-rho2);
surfaceScalarField rhoCpPhi = alphaPhi*(rho1*cp1-rho2*cp2)+phi*rho2*cp2;

// source terms due to mass sources (due to non divergence-free velocity field)
hCSource = fvc::ddt(rhoCp) + fvc::div(rhoCpPhi);

// energy equation
fvScalarMatrix TEqn
(
	fvm::ddt(rhoCp,T) + fvm::div(rhoCpPhi,T) - fvm::laplacian(k,T) - fvm::Sp(hESource,T) - fvm::Sp(hCSource,T) == -hESource*Tsat
);

TEqn.solve();
