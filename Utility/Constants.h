
//!  Constants containing file
 
const double C=1; //!< The capacity
const double Delay=1.5; //!< Delay for the transmission of a spike between pre- and postsynaptic neurons in [mS]
const double epsilon = 0.1; //!< constant << 1 : Ce = epsilon * Ne, Ci = epsilon * Ni
const double h=0.1; //!< Duration of a timeStep in [ms], T=n*h
const unsigned int DelayInSteps = static_cast<unsigned long>(ceil(Delay/h)); //!< Delay in timesteps
const unsigned int tau=20; //!< C*R [ms]
const double Je=0.1; //!< Amplitude of the signal sent by a postsynaptic excitatory neuron in [mV]
const unsigned int jToAddLength = DelayInSteps + 1; //!< Length of the vector jToAdd_
const double R=(tau/C); //!< The resistence
const double refractoryTime=2; //!< Time during which the membrane potential stays at 0mV,  in [ms]
const double refractoryTimeStep = static_cast<unsigned long>(ceil(refractoryTime/h)); //!< refractoryTime in timeSteps
const double scalarCste1 = exp(-h/tau); //!< Used in the membrane equation
const double scalarCste2 = R*(1-scalarCste1); //!< Used in the membrane equation
const unsigned int threshold=20; //!< Threshold beyond which the presynaptic will spike [mV]
const unsigned int Vreset=0; //!< The membrane potential reset to 0 [mV] after the refractory time
