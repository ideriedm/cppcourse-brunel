#include "network.hpp"
#include "neuron.hpp"



void Network::createNetwork(){
   
    //Ensures no other network has been made before this one and that the 2 vectors have the appropriate length
    neurons.resize(0);
    neurons.resize(getNbNeurons());
    neuronConnections_.resize(0);
    neuronConnections_.resize(getNbNeurons());
    
    
    //Test if the number of excitatory neurons is more than 0
    assert(getNbExcitatory()>1);
    //Test if the number of inhibitory neurons is more than 0
    assert(getNbInhibitory()>1);
    //Ensures that the number of excitatory + inhibitory = the number total of neurons
    assert(getNbInhibitory()+getNbExcitatory()==getNbNeurons());
    assert(getNbExcitatory()>getNbInhibitory());
    
    //Number of excitatory connections each neuron receives
    double ce(epsilon*getNbExcitatory());
    //Number of inhibitory connections each neuron receives
    double ci(epsilon*getNbInhibitory());
    
    //Creation of nbExcitatory neurons
    for(size_t i(0) ; i < getNbExcitatory() ; ++i){

        //Creation of an excitatory neuron with background noise
        Neuron* neuron(new Neuron(true));
        //Puts neuron in the vector of neurons
        neurons[i] = neuron;
    }

    //Creation of nbInhibitory neurons
    for(size_t i(getNbExcitatory()) ; i < getNbNeurons() ; ++i){

        //Creation of an inhibitory neuron with background noise
        Neuron* neuron(new Neuron(false));
        //Put neuron in the vector of neurons
        neurons[i] = neuron;
    }

    //Creation of the links between neurons
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distributionExcitatory(0,getNbExcitatory()-1);
    std::uniform_int_distribution<int> distributionInhibitory(getNbExcitatory(), neurons.size()-1);

    //Iterates on every neurons
    for(size_t idxNeuron(0); idxNeuron < getNbNeurons() ; ++idxNeuron){

        //Randomly chooses 0.1*getNbExcitatory excitatory neurons of index idx that will have "idxNeuron" in their targets
        for(size_t j(0); j < ce ; ++j){
            size_t idx = distributionExcitatory(gen);
            //Adds the target of index "idxNeuron" to the targets of neuron idx
            neuronConnections_[idx].push_back(idxNeuron);
        }
        //Randomly chooses 0.1*getNbInhibitory inhibitory neurons that will have "idxNeuron" in their targets
        for(size_t j(0); j < ci ; ++j){
            size_t idx = distributionInhibitory(gen);
            //Add the target of index "idxNeuron" to the targets of neuron idx
            neuronConnections_[idx].push_back(idxNeuron);
        }
        
    }
    
}



/*********************************************************************/

bool Network::getBackgroundNoise() const{
    
    return BackgroundNoise_;
}
unsigned int Network::getCe() const{
    return Ce_;
}

double Network::getEta() const{
    
    return Eta_;
}

double Network::getG() const{
    
    return g_;
}

unsigned long int Network::getGlobalClock() const{
    
    return GlobalClock_;
}

size_t Network::getJidxToRead() const{
    
    return jIdxToRead_;
}
size_t Network::getJidxToWrite() const{
    
    return jIdxToWrite_;
}
unsigned long int Network::getNbNeurons() const{
    
    return nbNeurons_;
}

unsigned long int Network::getNbExcitatory() const{
    
    return nbExcitatory_;
}
unsigned long int Network::getNbInhibitory() const{
    
    return nbInhibitory_;
}
double Network::getVext() const{
    return Vext_;
}

                       
/*********************************************************************/

Network::Network(bool const& backgroundNoise, double const& g, double const& Eta, double const& nbNeurons)
: BackgroundNoise_(backgroundNoise), g_(g), GlobalClock_(0), jIdxToRead_(0), jIdxToWrite_ (DelayInSteps), nbNeurons_(nbNeurons), Eta_(Eta)
{
    // Open the stream that writes the time at which a neuron spikes and its ID
    spikes.open("../result/spikes");
    assert(!spikes.fail());
    // Initialisation of the random devices
    std::random_device rd;
    randomGen_ = std::mt19937(rd());
    
    if(getNbNeurons() > 50){
        //Set the number of excitatory and inhibitory
        setNbExcitatory(0.8*getNbNeurons());
        setNbInhibitory(0.2*getNbNeurons());
        unsigned int ce =epsilon*getNbExcitatory();
        setCe(ce);
        
    }
    
    if(getBackgroundNoise()){
       
        // Calculation of the Vext, from the Vratio given (Vratio = Vext/Vthr)
        // Vthr = threshold/(Ce*J*tau) is the frequency at which the neuron spikes, in absence of backgroundNoise
        double Vthr(threshold/(getCe()*Je*tau));
        // Vext = Eta*Ce*Vthr
        setVext(getEta()*getCe()*Vthr);
        
        // Initialisation of the poisson distribution
        poissonDistr_ = std::poisson_distribution<>(getVext()*h);

    }

}

Network::~Network(){
    
    assert(!neurons.empty());
    //deallocation of the memory
    for(auto neuron : neurons){
        if(neuron!=nullptr)
            delete neuron;
    }
    
    //Close the file at the end of the simulation
    spikes.close();
    
}

void Network::setCe(unsigned int& ce){
    
    Ce_=ce;
}

void Network::setNbExcitatory(unsigned long int const& nb){
    
    nbExcitatory_= nb;
}

void Network::setNbInhibitory(unsigned long int const& nb){
    
    nbInhibitory_= nb;
}

void Network::setVext(double const& newV){
    
    Vext_ = newV;
}
                       
/*********************************************************************/


void Network::updateJIndex(){
    
    ++jIdxToRead_;
    //The index is back to 0
    if(jIdxToRead_ > DelayInSteps)
        jIdxToRead_ = 0;

    ++jIdxToWrite_;
    if(jIdxToWrite_ > DelayInSteps)
        jIdxToWrite_ = 0;
    
}

void Network::updateNetwork(double const& StartStep, double const& StopStep){
    
    //Some verifications
    assert(StartStep >= 0);
    assert(StopStep > 0);
    assert(StartStep < StopStep);
    
    //Check if there are neurons in the network
    assert(!neurons.empty());
    
    //The simulation stops at StopStep
    while(getGlobalClock() < StopStep){
        
        for(size_t NeuronIndice(0) ; NeuronIndice < getNbNeurons() ; ++NeuronIndice){
            
                //Add the backgroundNoise to the buffer
                if(getBackgroundNoise())
                    neurons[NeuronIndice] ->jToAdd_[jIdxToRead_] += poissonDistr_(randomGen_);
            
                //Update each neuron and give the index at which the neuron corresponding to neuronIndice should check its buffer
                bool spike(neurons[NeuronIndice]->update(jIdxToRead_, getGlobalClock()));
            
                if(spike){
                    //Iteration on all neuron's targets
                    for(auto targetIndice : neuronConnections_[NeuronIndice] ){
                        //If has spiked, will send an action potential and stock it into the buffer of the target, at the index jIdxToWrite
                        if(neurons[NeuronIndice]->getIsExcitatory())
                            //1 if excitatory
                            neurons[targetIndice]->jToAdd_[jIdxToWrite_] += 1;
                        else
                            //-g if inhibitory
                            neurons[targetIndice]->jToAdd_[jIdxToWrite_] += - getG();
                    }
                    
                    //write the time and the id of the neuron that has spiked into a file
                    if(getGlobalClock() > StartStep)
                        spikes << getGlobalClock() << " " << NeuronIndice << std::endl ;
                }
            }
        
        //The global clock updates after all the neurons already have
        updateTime();
        //The indexes are updated too
        updateJIndex();
    }
    
    
}

void Network::updateTime(){
    
    ++GlobalClock_;
    
}
