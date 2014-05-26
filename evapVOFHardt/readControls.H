#   include "readTimeControls.H"
#   include "readPISODict.H"

    bool correctPhi = true;
    if (pisoDict.found("correctPhi"))
    {
        correctPhi = Switch(pisoDict.lookup("correctPhi"));
    }

    bool checkMeshCourantNo = false;
    if (pisoDict.found("checkMeshCourantNo"))
    {
        checkMeshCourantNo = Switch(pisoDict.lookup("checkMeshCourantNo"));
    }
