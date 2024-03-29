#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"
#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"

#include "qsimOpticalPhysics.hh"

//#include "G4UserSpecialCuts.hh"

qsimOpticalPhysics::qsimOpticalPhysics(G4bool toggle) : G4VPhysicsConstructor("Optical") {
    theWLSProcess                = NULL;
    theScintProcess              = NULL;
    theCerenkovProcess           = NULL;
    theBoundaryProcess           = NULL;
    theAbsorptionProcess         = NULL;
    theRayleighScattering        = NULL;
    theMieHGScatteringProcess    = NULL;
    
    AbsorptionOn                 = toggle;
}

qsimOpticalPhysics::~qsimOpticalPhysics() { }


void qsimOpticalPhysics::ConstructParticle() {
    G4OpticalPhoton::OpticalPhotonDefinition();
}

void qsimOpticalPhysics::ConstructProcess() {
    /*
        FIXME:  Add to verbosity responsiveness
        G4cout << "qsimOpticalPhysics:: Add Optical Physics Processes" << G4endl;
    */
    
    theWLSProcess = new G4OpWLS();
    
    theScintProcess = new G4Scintillation();
    //theScintProcess->SetScintillationYieldFactor(1.);
    theScintProcess->SetTrackSecondariesFirst(true);
    
    theCerenkovProcess = new G4Cerenkov();
    theCerenkovProcess->SetMaxNumPhotonsPerStep(2000);
    theCerenkovProcess->SetTrackSecondariesFirst(true);
    
    theAbsorptionProcess      = new G4OpAbsorption();
    theRayleighScattering     = new G4OpRayleigh();
    theMieHGScatteringProcess = new G4OpMieHG();
    theBoundaryProcess        = new G4OpBoundaryProcess();
    
    G4ProcessManager* pManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
    //pManager->AddProcess(new G4UserSpecialCuts(),-1,-1,1);
    
    if (!pManager) {
        std::ostringstream o;
        o << "Optical Photon without a Process Manager";
        G4Exception("qsimOpticalPhysics::ConstructProcess()","", FatalException,o.str().c_str());
    }

    if (AbsorptionOn) pManager->AddDiscreteProcess(theAbsorptionProcess);
    
    pManager->AddDiscreteProcess(theRayleighScattering);
    pManager->AddDiscreteProcess(theMieHGScatteringProcess);
    
    pManager->AddDiscreteProcess(theBoundaryProcess);
    
    theWLSProcess->UseTimeProfile("delta");
    //theWLSProcess->UseTimeProfile("exponential");
    
    pManager->AddDiscreteProcess(theWLSProcess);
    
    //theScintProcess->SetScintillationYieldFactor(1.);
    //theScintProcess->SetScintillationExcitationRatio(0.0);
    theScintProcess->SetTrackSecondariesFirst(true);
    
    // Use Birks Correction in the Scintillation process
    
    G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
    theScintProcess->AddSaturation(emSaturation);
    
    auto aParticleIterator=GetParticleIterator(); // NEW FOR GEANT4.10.4. Erase for older versions.
    aParticleIterator->reset();

    while ( (*aParticleIterator)() ){
        G4ParticleDefinition* particle = aParticleIterator->value();
        G4String particleName = particle->GetParticleName();
        
        pManager = particle->GetProcessManager();
        if (!pManager) {
            std::ostringstream o;
            o << "Particle " << particleName << "without a Process Manager";
            G4Exception("qsimOpticalPhysics::ConstructProcess()", "", FatalException,o.str().c_str());
        }

        if(theCerenkovProcess->IsApplicable(*particle)){
            pManager->AddProcess(theCerenkovProcess);
            pManager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
        }

        if(theScintProcess->IsApplicable(*particle)){
            pManager->AddProcess(theScintProcess);
            pManager->SetProcessOrderingToLast(theScintProcess,idxAtRest);
            pManager->SetProcessOrderingToLast(theScintProcess,idxPostStep);
        }
    }
}

void qsimOpticalPhysics::SetNbOfPhotonsCerenkov(G4int MaxNumber) {
    theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumber);
}