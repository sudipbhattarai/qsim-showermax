# Output tree variables

The output trees are organized into several indepdent arrays

ev.*  - generated particle info, one entry per particle
hit.* - detector hits
sci.* - scintillator detector sums


## event data - single variables
ev.pid		Geant4 particle type
ev.v[xyz]	creation vertex, lab frame [m]
ev.p		Particle initial momentum [GeV]
ev.p[xyz]	Particle initial momentum components, lab frame [GeV]
ev.th		Particle initial polar angle [rad]
ev.ph		Particle initial azimuthal angle [rad]


## hit data
hit.n		Number of hits for the event, number of electrons emitted 
				from cathode for event n
hit.det		Detector number (2 is the PMT)
hit.vid		Volume ID number (not yet implemented)
hit.pid		Geant4 particle type
hit.trid	Geant4 track ID number (1 = first particle created)
hit.mtrid	Geant4 mother track ID number (0 = particle from gun)
hit.gen		Generator (not yet implemented)
hit.[xyz]	Hit coordinate, lab frame [m]
hit.p		Momentum magnitude of particle [GeV]
hit.p[xyz]	Momentum components of particle, lab frame [GeV]
hit.v[xyz]	Creation vertex of particles
hit.e		Energy of particle [GeV]
hit.m		Mass of particle [GeV]

## scint data
sci.n		Number of hits for the event
sci.det		Detector number (1 top only, 2 bottom only, 10 for both)
sci.vid		Volume ID number (not yet implemented)
sci.edep	Energy deposited [GeV]


