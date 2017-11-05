#include "neuron.hpp"

double Neuron::getI() const {
    
    return I_;
}

bool Neuron::getIsExcitatory() const{
    
    return isExcitatory_;
}

double Neuron::getMembranePotential() const {
	
	return membranePotential_;
}

/*********************************************************************/

Neuron::Neuron(bool const& isExcitatory)

: I_(0), isExcitatory_(isExcitatory), membranePotential_(0), timeSpike_(1000), jToAdd_(jToAddLength, 0)
{}


/*********************************************************************/


void Neuron::setI(double const& I){
    
    I_ = I;
}

void Neuron::setIsExcitatory(bool const& b){
    
    isExcitatory_ = b;
}


void Neuron::setMemPot(double const& NewPotential){
	
	membranePotential_=NewPotential;
	
}


/*********************************************************************/

bool Neuron::update(size_t const& Jidx, int const& time){
    
    // Contains the number of spikes this* should add to its membrane potential at the current time
    double nbSpikes = jToAdd_[Jidx];
    // Empty the corresponding case of the buffer after reading it
    jToAdd_[Jidx]=0;
    
    // Boolean that the method will return. For the moment, it equals false, because the neuron hasn't spiked
    bool Spike(false);
    

    // This* is not in a refractory state, therefore, we should check some conditions
    if (!(std::abs(time-timeSpike_) < refractoryTimeStep)){
        
        // If the membrane potential is bigger than the threshold, this* will spike
        if(getMembranePotential() >= threshold){
            
            /* After a certain threshold, the neuron will send an
             * action potential and reach a refractory state .
             * Then the membrane potential become Vreset */
            setMemPot(Vreset);
            // Spike equals true because this* has spiked
            Spike=true;
            timeSpike_= time;
        }
        else {
            // The membrane potential changes according to the chosen equation
            MembraneEquation(getI(), nbSpikes);
        }
    }
    
    
    // Return true if there are spikes in the time interval
    return Spike;
    
}

void Neuron::MembraneEquation(double const& i, double const& nbSpikes){
    
    // Equation for the membrane potential
    setMemPot( scalarCste1*getMembranePotential() + i*scalarCste2 + nbSpikes*Je);
    
}

