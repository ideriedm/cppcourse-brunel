#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <fstream>
#include "neuron.hpp"


//!  Class Network
/*!
 This class models a network of many neurons. It creates as many neurons as you want to, handle the globalClock of the simulation and update itself (so it updates all the neurons of the simulation) of the number of timeSteps you want.
 It handles all the connections between the neuron with its attribute neuronConnections. It contains all the neurons of the simulation (in form of index), and each of them has a vector of indexTarget (index that correspond to targets). It also has a vector with all the neurons (pointers).
 During the update, it handles the Poisson distribution and fills the buffer of the neurons.
 */

class Network{
    
    const double epsilon = 0.1; //!< constant << 1 : Ce = epsilon * Ne, Ci = epsilon * Ni

    private:

    bool BackgroundNoise_ ; //!< False if we want to test the program without the background noise
    double g_; //!< Ratio Ji/Je
    unsigned int Ce_; //!< Number of excitatory connections each neuron receives
    unsigned long int GlobalClock_; //!< The global time in timeSteps
    size_t jIdxToRead_ ; //!< Indice of the buffer where we read (corresponding to the current time). Equals 0 at the construction
    size_t jIdxToWrite_ ; //!< Indice of the buffer where we write (corresponding to the current time + delay). Equals delayInSteps at the construction
    unsigned long int nbExcitatory_; //!< Number of excitatory neurons that we want to simulate
    unsigned long int nbInhibitory_; //!< Number of inhibitory neurons that we want to simulate
    unsigned long int nbNeurons_; //!< Total number of neurons that we want to simulate
    double Vext_; //!< Frequency at which Ce "artificial" neurons spike
    double Eta_; //!< Ratio Vext/Vthr
    
    std::mt19937 randomGen_; //!< Random generator, used for the Poisson distribution
    std::poisson_distribution<> poissonDistr_; //!< The Poisson distribution used in the membrane equation, to simulate 1000 neurons spiking randomly

    
    std::ofstream spikes; //!< Stream to write the time and the ID of each neuron that has spiked (in the file ../result/spikes)

    
    /*********************************************************************/
    
    /**
     * @return BackgroundNoise_
     */
    bool getBackgroundNoise() const;
    /**
     * @return Ce_
     */
    unsigned int getCe() const;
    /**
     * @return Eta_
     */
    double getEta() const;
    
    
    
    
    /*********************************************************************/
    /**
     * Setter of Ce_
     * @param ce is the new Ce_
     */
    void setCe(unsigned int& ce);
    /**
     * Setter of nbExcitatory_
     * @param nb is the number of excitatory neurons you want
     */
    void setNbExcitatory(unsigned long int const& nb);
    
    /**
     * Setter of nbInhibitory_
     * @param nb is the number of inhibitory neurons you want
     */
    void setNbInhibitory(unsigned long int const& nb);
    /**
     * Setter for the Vext_
     * @param newV is the new Vext_
     */
    void setVext(double const& newV);
    
    /*********************************************************************/

    
    /*********************************************************************/
    //PUBLIC PART
    /*********************************************************************/
    
    
    public :
    
    std::vector< std::vector<size_t> > neuronConnections_; //!< Each neuron index has a vector containing the idx of its targets. Breaks the encapsulation a little bit but enables the tests to be run more easily
    std::vector<Neuron*> neurons; //!< Contains all the neurons of the simulation. The getNbexcitatory first are excitatory, and the rest are inhibitory. Breaks the encapsulations but enables the tests to be run more easily
    
    /**
     * Creates nbNeurons_ neurons, decides which one are excitatory or inhibitory. Handles the connections between them. Load all the neurons in the attribute neurons.
     */
    void createNetwork();
    
    /**
     * Method only usefull in the tests, to create and connect 2 neurons without background noise. The second neuron will be a target of the first one
     */
    void connectTwoNeurons();
    /**
     * Getter for the g_
     * @return g_
     */
    double getG() const;
    /**
     * Getter for the clock_
     * @return clock_
     */
    unsigned long int getGlobalClock() const;
    /**
     * Getter for the jIdxToRead_
     * @return jIdxToRead_
     */
    size_t getJidxToRead() const;
    /**
     * Getter for the jIdxToWrite_
     * @return jIdxToWrite_
     */
    size_t getJidxToWrite() const;
    /**
     * Getter for the nbExcitatory_
     * @return nbExcitatory_
     */
    unsigned long int getNbExcitatory() const;
    /**
     * Getter for the nbInhibitory_
     * @return nbInibitory_
     */
    unsigned long int getNbInhibitory() const;
    /**
     * Getter for the nbNeurons_
     * @return nbNeurons_
     */
    unsigned long int getNbNeurons() const;
    /**
     * @return Vext_
     */
    double getVext() const;
    

    
    /*********************************************************************/
    
    /**
     * Constructor of a network
     * @param g : ratio Ji/Je
     * @param Eta : ratio Vext/Vthr
     * @param nbNeurons is the number of neurons we want to simulate
     */
    Network(bool const& backgroundNoise, double const& g, double const& Eta, double const& nbNeurons);
    
    /**
     * Destructor of a network, have to delete all the pointers it contained
     */
    ~Network();
    
    /*********************************************************************/
    
    /**
     * Increase circularly the index from jToAdd_
     */
    void updateJIndex();
    
    /**
     * Update the network: call the method update of each neurons of the simulation
     * @param Starstep : beginning of the time interval for the graph
     * @param StopStep : end of the time interval for the graph
     */
    void updateNetwork(double const& StartStep, double const& StopStep);
    
    /**
     * Increases the global clock_ of one TimeStep h
     */
    void updateTime();
};

#endif /* network_hpp */
