Description {#mainpage}
=======================

## Neuro project, Brunel_2000 simulation

## Aim:

The aim of the simulation is to create a network of many neurons and to show how they interact together with a graph (similar to figure 8, Brunel's publication)

## The class Neuron:

This class models a neuron with a certain number of caracteristics.

## The membrane potential:
Beginning at 0 [mV], it could be increased or decreased, as it receives electric impulsions. An electric impulsion sent by a presynaptic neuron is called a spike or action potential. Each time a postsynaptic neuron receives a spike (after a certain delay), its membrane potential will be increased by a certain amount j (j depends if the neuron source is inhibitory or excitatory). The membrane potential could also be increased by an external current Iext, but that is not observed practically (we implement it at the beginning to enable the simulation and also for the tests, to see if a neuron behaves correctly). If the membrane potential exceeds a certain threshold, the presynaptic neuron will spike and send a certain number of spikes to all its target (postsynaptic) neurons (1 if excitatory, -g if inhibitory). In reaction to the action potential, its membrane potential will be set to Vreset and it will stay in a refractory state for a certain time (refractoryTime).

## The connections:
Each neuron receives a number Ce and Ci of excitatory and inhibitory connections. That means that each neuron is in the targets of Ce excitatory and Ci inhibitory neurons.It is also possible to have multiple connections between two neurons, and a neuron can be bound to itself. To simulate the noise, Cext (=Ce) connections with excitatory neurons outside the network will be create. In an easier way, Cext artificial neurons will spike with a frequency vext, according to the poisson_distribution.

## Update the membrane potential :
A post synaptic neuron can receive more than one spike at a time, from more than one neuron.  To handle all these incoming informations, we set up a system of buffer (here called jToAdd_). At each time t, the post synaptic neuron will add all the j it has received and put it in the corresponding case of jToAdd_ . Concretely, it will put the number of spikes at the case corresponding to time T+delay.
At the beginning of the update, each neuron will look into its buffer (at the case corresponding to the current time) and check if there are spikes to add to its membrane potential. If it is the case, it will add them and empty the box.

A neuron can be initialized with 1 arguments, if it is excitatory or inhibitory
A neuron has a methods to update itself, to calculate the membrane potential and some getters and setters.

## The class Network:

This class models a network of many neurons. It creates as many neurons as you want to, handle the globalClock of the simulation and update itself (so it updates all the neurons of the simulation) of the number of timeSteps you want.
It handles all the connections between the neuron with its attribute neuronConnections. It contains all the neurons of the simulation (in form of index), and each of them has a vector of indexTarget (index that correspond to targets). It also has a vector with all the neurons (pointers).
During the update, it handles the Poisson distribution and fills the buffer of the neurons.

## The file Constants.h :



