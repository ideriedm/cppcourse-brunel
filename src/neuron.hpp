#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include <cassert>
#include "../Utility/Constants.h"




//!  Class Neuron
/*!
 This class models a neuron with a certain number of caracteristics.
 
 The membrane potential:
Beginning at 0 [mV], it could be increased or decreased, as it receives electric impulsions. An electric impulsion sent by a presynaptic neuron is called a spike or action potential. Each time a postsynaptic neuron receives a spike (after a certain delay), its membrane potential will be increased by a certain amount j (j depends if the neuron source is inhibitory or excitatory). The membrane potential could also be increased by an external current Iext, but that is not observed practically (we implement it at the beginning to enable the simulation and also for the tests, to see if a neuron behaves correctly). If the membrane potential exceeds a certain threshold, the presynaptic neuron will spike and send a certain number of spikes to all its target (postsynaptic) neurons (1 if excitatory, -g if inhibitory). In reaction to the action potential, its membrane potential will be set to Vreset and it will stay in a refractory state for a certain time (refractoryTime).
 
 The connections:
 Each neuron receives a number Ce and Ci of excitatory and inhibitory connections. That means that each neuron is in the targets of Ce excitatory and Ci inhibitory neurons.It is also possible to have multiple connections between two neurons, and a neuron can be bound to itself. To simulate the noise, Cext (=Ce) connections with excitatory neurons outside the network will be create. In an easier way, Cext artificial neurons will spike with a frequency vext, according to the poisson_distribution.
 
 Update the membrane potential :
 A post synaptic neuron can receive more than one spike at a time, from more than one neuron.  To handle all these incoming informations, we set up a system of buffer (here called jToAdd_). At each time t, the post synaptic neuron will add all the j it has received and put it in the corresponding case of jToAdd_ . Concretely, it will put the number of spikes at the case corresponding to time T+delay.
 At the beginning of the update, each neuron will look into its buffer (at the case corresponding to the current time) and check if there are spikes to add to its membrane potential. If it is the case, it will add them and empty the box.
 
 A neuron can be initialized with 1 arguments, if it is excitatory or inhibitory
 A neuron has a methods to update itself, to calculate the membrane potential and some getters and setters.
 */

class Neuron{
    
    private:

    /*********************************************************************/
            //ATTRIBUTS!!
    /*********************************************************************/

    double I_; //!< The external current
    bool isExcitatory_; //!< True if the neuron is excitatory, false if it is inhibitory
	double membranePotential_; //!< The membrane potential in [mV]
    double timeSpike_; //!< Time at which a spike occures

    
	
    /*********************************************************************/

    
    /**
     * @return I_
     */
    double getI() const;
    
    /*********************************************************************/
    
    /**
     * Determines the new membrane potential according to an equation
     * @param I is the external current
     * @param nbSpikes is the number of spikes to add
     */
    void MembraneEquation(double const& I, double const& nbSpikes);
    
    
    /*********************************************************************/

    /**
     * Setter for the membranePotential_
     * @param NewPotential is the new MemPotential_
     */
    void setMemPot(double const& NewPotential);


    /*********************************************************************/
    //PUBLIC PART
    /*********************************************************************/
    
	public:

    std::vector<int> jToAdd_; //!< Our buffer. Vector containing the number of spikes that should be added to the membrane potential. Each cases (DelayInStep + 1 cases) corresponds to one TimeStep. If a spike is sent at timestep 0, the postsynaptic neuron will receive it at timestep 15, in this case. Even though it breaks the encapsulation a little bit, putting jToAdd_ in public enables to reduce the simulation time. Indeed, we use it directly in network. An excitatory will send 1 spike, an inhibitory will send -g spike
    

    /**
     * @return isExcitatory_
     */
    bool getIsExcitatory() const;
    /**
     * @return MembranePotential_
     */
    double getMembranePotential() const;

    
    /*********************************************************************/
    
	/**
     * Constructor of Neuron
     * @param isExcitatory : equals true if the neuron is excitatory
     */
    Neuron (bool const& isExcitatory=true);
	

    /*********************************************************************/
    
    /**
     * Setter for the BackgroundNoise_
     * @param b is a boolean for the new state of backgroundNoise_
     */
    void setBackgroundNoise(bool const& b);
    /**
     * Setter for the current I_
     * @param I is the new I_
     */
    void setI(double const& I);
    
    /**
     * Setter for the boolean isExcitatory_
     * @param b is a boolean for the new state of isExcitatory_
     */
    void setIsExcitatory(bool const& b);

    
    /*********************************************************************/
    
	/**
     * Update from time t to time t+T (T=N*h)
     * Handle the PauseTime and the conditions for a neuron to spike
     * Read the buffer jToAdd_ and add the corresponding number of spikes to the membraneEquation
     * Update the membraneEquation_
     * @param Jidx is the index at which this* should read its buffer
     * @param time is the global time
     * @return true if there is a spike in the time interval
     */
	bool update(size_t const& Jidx, int const& time=0);
    

};

#endif
