#include "neuron.hpp"
#include "network.hpp"
#include "gtest/gtest.h"

/**
 * Test the membrane potential of 1 neuron after 1 timeStep
 */
TEST (NeuronTest, MembranePotential) {
    
    //Creation of a neuron without background noise
    Neuron neuron(false);
    //Set Iext to 1.0
    neuron.setI(1.0);
    
    neuron.update(0);
    //Comparison between the 2 expressions after 1 update
    EXPECT_EQ(20*(1.0-exp(-0.1/20.0)), neuron.getMembranePotential());
    
}

/**
 * Test the membrane potential corresponding to an input of 1.0 (should tend to 20 [mV] but never reach it) and its behavior when you remove the external input (should tend back to 0)
 */
TEST (NeuronTest, PositiveInput){
    
    //Creation of a neuron without background noise
    Neuron neuron(false);
    //Set Iext to 1.0
    neuron.setI(1.0);
    
    int nbSpikes(0);
    int clock(0);
    //Update the neuron (10'000 times)
    for(size_t i(0) ; i<10000 ; ++i){
        if(neuron.update(0,clock))
            ++nbSpikes;
        ++clock;
    }
    
    //The neuron should not spike with a current of 1.0
    EXPECT_EQ(0, nbSpikes);
    //The membrane potential should tend to 20mV but never reach it
    EXPECT_GT(1E-3, fabs(20-neuron.getMembranePotential()));
    
    //With no external current, the membrane potential should go back to 0 (tends toward 0)
    neuron.setI(0.0);
    //We wait enough time for it
    for(size_t i(0) ; i < 10000 ; ++i){
        neuron.update(0,clock);
        ++clock;
    }
    //Because it is an exponential damping, it will never exactly reach 0, so you expect the membrane potential to be near 0.
    EXPECT_NEAR(0,neuron.getMembranePotential(),1E-3);
    
}

/**
 * Test the membrane potential corresponding to an input of -1.0 (should tend to -20 [mV] but never reach it) and its behavior when you remove the external input (should tend back to 0)
 */
TEST (NeuronTest, NegativeInput){
    
    //Creation of a neuron without background noise
    Neuron neuron(false);
    //Set Iext to 1.0
    neuron.setI(-1.0);
    
    int nbSpikes(0);
    int clock(0);
    //Update the neuron (10'000 times)
    for(size_t i(0) ; i<10000 ; ++i){
        if(neuron.update(0,clock))
            ++nbSpikes;
        ++clock;
    }
    
    //The neuron should not spike with a current of -1.0
    EXPECT_EQ(0, nbSpikes);
    //The membrane potential should tend to -20mV
    EXPECT_GT(1E-3, fabs(-20-neuron.getMembranePotential()));
    
    //With no external current, the membrane potential should go back to 0 (tends toward 0)
    neuron.setI(0.0);
    //We wait enough time for it
    for(size_t i(0) ; i < 10000 ; ++i){
        neuron.update(0,clock);
        ++clock;
    }
    //Because it is an exponential damping, it will never exactly reach 0, so you expect the membrane potential to be near 0.
    EXPECT_NEAR(0,neuron.getMembranePotential(),1E-3);
    
   
}

/**
 * Test the times at which a single neuron should spike with an input of 1.01
 */
TEST (NeuronTest, TimeSpike){
    
    //Creation of a neuron without background noise
    Neuron neuron(false);
    //Set Iext to 1.0
    neuron.setI(1.01);
    
    //According to the data, the neuron should spike at time :
    //92.4 ms, 186.8ms, 281.2 ms, 375.6ms
    
/******************************************************************************/
    //Waiting until the first spike
/******************************************************************************/
    int nbSpikes(0);
    int clock(0);
    for(; clock < 924 ; ++clock){
        if(neuron.update(0,clock))
        ++nbSpikes;
    }

    
    //Step 924
    EXPECT_EQ(0, nbSpikes);
    //Should spike one update later (Step 925)
    if(neuron.update(0,clock))
        ++nbSpikes;
    ++clock;

    
    //Should spike at Time 92.4ms (after 925 steps)
    EXPECT_EQ(1,nbSpikes);
    //The membrane potential should be 0 right after the spike
    EXPECT_EQ(0,neuron.getMembranePotential());
    
/******************************************************************************/
    //Waiting for the second spike
/******************************************************************************/
    //186.8-92.4=94.4 [ms] => 944 TimeSteps
    for(; clock < 1868 ; ++clock){
        if(neuron.update(0,clock))
            ++nbSpikes;
    }
    
  
    EXPECT_EQ(1,nbSpikes);

    if(neuron.update(0,clock))
        ++nbSpikes;
    ++clock;


    EXPECT_EQ(2,nbSpikes);

/******************************************************************************/
    //Waiting for the third spike
/******************************************************************************/
    //281.2-186.8=94.4 [ms] => 944 TimeSteps
    for(; clock < 2812 ; ++clock){
        if(neuron.update(0,clock))
            ++nbSpikes;

    }


    EXPECT_EQ(2,nbSpikes);

    if(neuron.update(0,clock))
        ++nbSpikes;
    ++clock;

    EXPECT_EQ(3,nbSpikes);

/******************************************************************************/
    //Waiting for the forth spike
/******************************************************************************/
    //375.6-281.2=94.4 [ms] => 944 TimeSteps
    for(; clock <3756 ; ++clock){
        if(neuron.update(0,clock))
            ++nbSpikes;
    }



    EXPECT_EQ(3,nbSpikes);

    if(neuron.update(0,clock))
        ++nbSpikes;
    ++clock;

    EXPECT_EQ(4,nbSpikes);

    
}

//! Class NetTest
/*!
 This class heritate from Network, but has a different update method and a method to specifically create and connect 2 neurons, for the purpose of the tests.
 */
class NetTest : public Network {
    
public:
    /*
     * Creation of a network of 2 neurons, with parameters g=5 and eta=2, without backgroundNoise
     */
    NetTest()
    : Network(false,5,2,2)
    {}
    
    /**
     * ConnectTwoNeurons creates 2 neurons and put neuron1 in the targets of neuron0
     */
    void connectTwoNeurons(){
        
        //Ensure that the vector neurons has a length of 2
        neurons.resize(2);
        //Create 2 neurons one excitatory and one inhibitory
        neurons[0] = new Neuron (true);
        neurons[1] = new Neuron (false);
        //Ensure that the neuronConnections_ has a length of 2
        neuronConnections_.resize(2);
        //Put neuron 1 in target of neuron 0
        neuronConnections_[0].push_back(1);
        
    }
    
    /**
     * Update for test is a modified version of update network adapted for 2 neurons.
     * @return a vector of length = 2 that contains the timeSpikes of the 2 neurons (if they have) and 0 otherwise.
     */
    std::vector<int> update_for_test() {
        
        std::vector<int> timeSpikes(neurons.size(),0);

        for(size_t NeuronIndice(0) ; NeuronIndice < getNbNeurons() ; ++NeuronIndice){
            
            //update each neuron
            bool spike(neurons[NeuronIndice]->update(getJidxToRead(),getGlobalClock()));
            if(spike){
                timeSpikes[NeuronIndice] = getGlobalClock();
                
                for(auto targetIndice : neuronConnections_[NeuronIndice] ){
                    if(neurons[NeuronIndice]->getIsExcitatory())
                        //if has spiked, will send an action potential
                        neurons[targetIndice]->jToAdd_[getJidxToWrite()] += 1;
                    else
                        neurons[targetIndice]->jToAdd_[getJidxToWrite()] += -getG();
                }
            }
        }
        
        updateJIndex();
        updateTime();

        return timeSpikes;
    }
};

/**
 * Test the connection (1 excitatory -neurons[0]- has in its target 1 neuron -neurons[1]-): If the first one spike, if the second one doesn't, if it receives correctly the output of neuron[0] with the correct delay
 */
TEST (TwoNeurons, spikeTransmissionFromExcitatory){

    NetTest net;
    net.connectTwoNeurons();
    net.neurons[0]->setI(1.01);

    bool spike1(false), spike2(false);
    int counter1(0);
    std::vector<int> timeSpikes(net.getNbNeurons(),0);
    timeSpikes={0,0};

    for(size_t i(0) ; i < 924 ; ++i){
        timeSpikes = net.update_for_test();
        if(timeSpikes[0]!=0){
            spike1 = true;
            ++counter1;
        }
        if(timeSpikes[1]!=0)
            spike2 = true;

        //the 2 neurons should not spike
        EXPECT_FALSE(spike1);
        EXPECT_FALSE(spike2);

    }

    //If they don't spike, the membrane potential of neuron 2 stays at 0mV
    EXPECT_EQ(0, counter1);
    EXPECT_EQ(0, net.neurons[1]->getMembranePotential());

    timeSpikes = net.update_for_test();
        if(timeSpikes[0]!=0){
            spike1 = true;
            ++counter1;
        }
        if(timeSpikes[1]!=0)
            spike2 = true;

    //Neuron0 should spike, neuron1 don't
    EXPECT_TRUE(spike1);
    EXPECT_FALSE(spike2);

    spike1=false;

    //Waiting for the delay to end
    for(size_t i(0) ; i < 15 ; i++){

        timeSpikes = net.update_for_test();
        if(timeSpikes[0]!=0){
            spike1 = true;
            ++counter1;
        }
        if(timeSpikes[1]!=0)
            spike2 = true;

        //The 2 neurons should not spike
        EXPECT_FALSE(spike1);
        EXPECT_FALSE(spike2);

    }


    //Neuron0 should not have spiked again
    EXPECT_EQ(1, counter1);
    //Delay has ended, neuron1 should receives the spike
    //Neuron1 receives the spike at time 92.4 + 1.5 = 93.9 (+0.1, because it updates at the end)
    EXPECT_NEAR(94, net.getGlobalClock()*h, 1E-3);
    //The membrane potential of neuron1 should be 0.1 because it receives the amplitude j from neuron0
    EXPECT_EQ(0.1, net.neurons[1]->getMembranePotential());

}

/**
 * Test the connection (1 inhibitory -neurons[0]- has in its target 1 neuron -neurons[1]-): If the first one spike, if the second one doesn't, if it receives correctly the output of neuron[0] with the correct delay
 */
TEST (TwoNeurons, spikeTransmissionFromInhibitor){

    NetTest net;
    net.connectTwoNeurons();
    net.neurons[0]->setIsExcitatory(false);
    net.neurons[0]->setI(1.01);

    bool spike1(false), spike2(false);
    int counter1(0);
    std::vector<int> timeSpikes(net.getNbNeurons(),0);
    timeSpikes={0,0};

    for(size_t i(0) ; i < 924 ; i++){
        timeSpikes = net.update_for_test();

        if(timeSpikes[0]!=0){
            spike1 = true;
            ++counter1;
        }
        if(timeSpikes[1]!=0)
            spike2 = true;

        //The 2 neurons should not spike
        EXPECT_FALSE(spike1);
        EXPECT_FALSE(spike2);

    }

    //If they don't spike, the membrane potential of neuron 2 stays at 0mV
    EXPECT_EQ(0, counter1);
    EXPECT_EQ(0, net.neurons[1]->getMembranePotential());

    timeSpikes =net.update_for_test();
    if(timeSpikes[0]!=0){
        spike1 = true;
        ++counter1;
    }
    if(timeSpikes[1]!=0)
        spike2 = true;

    //Neuron0 should spike, neuron1 don't
    EXPECT_TRUE(spike1);
    EXPECT_FALSE(spike2);

    spike1 = false;

    //Waiting for the delay to end
    for(size_t i(0) ; i < 15 ; i++){

        timeSpikes = net.update_for_test();

        if(timeSpikes[0]!=0){
            spike1 = true;
            ++counter1;
        }
        if(timeSpikes[1]!=0)
            spike2 = true;

        //The 2 neurons should not spike
        EXPECT_FALSE(spike1);
        EXPECT_FALSE(spike2);

    }


    //Neuron0 should not have spiked again
    EXPECT_EQ(1, counter1);
    //Delay has ended, neuron1 should receives the spike
    //Neuron1 receives the spike at time 92.4 + 1.5 = 93.9 (+0.1, because it updates at the end)
    EXPECT_NEAR(94, net.getGlobalClock()*h, 1E-3);
    //The membrane potential of neuron1 should be -0.5 because it receives the amplitude j from neuron0
    EXPECT_EQ(-0.5, net.neurons[1]->getMembranePotential());


}

/**
 * Test the creation of a network of 12500 : if there is the right number of total neurons, if there is the right number of excitatory and inibitory ones, counts the number of excitatory and inibitory connections each neuron receives
 */
TEST (Network, creation){

    //Creation of a network of 12500 neurons, without backgroundNoise
    Network net(true,3, 2, 12500);
    net.createNetwork();

    //Check if the numbers of neurons are correct
    EXPECT_EQ(12500, net.getNbNeurons());
    EXPECT_EQ(10000, net.getNbExcitatory());
    EXPECT_EQ(2500, net.getNbInhibitory());
    EXPECT_EQ(20,net.getVext());
    //Argument of poisson
    EXPECT_EQ(2,net.getVext()*h);
    
    double counter(0);
    for(size_t i(0); i < net.getNbExcitatory() ; ++i){
        for(size_t j(0); j < net.neuronConnections_[i].size(); ++j){
            if(net.neuronConnections_[i][j]==1)
                ++counter;
        }
    }
    EXPECT_EQ(1000, counter);
    counter=0;
    for(size_t i(net.getNbExcitatory()); i < net.getNbNeurons() ; ++i){
        for(size_t j(0); j < net.neuronConnections_[i].size(); ++j){
            if(net.neuronConnections_[i][j]==1)
                ++counter;
        }
    }
    EXPECT_EQ(250, counter);

}

/**
 * Test if the parameters of net are taken in account correctly.
 */
TEST(Neuron, variables){
    
    //Create a network with g=5, eta=2 and 10 neurons, without backgroundNoise
    Network net(false, 5,2,100);
    net.createNetwork();

    //Check G and Vext for one of those neurons
    EXPECT_EQ(5,net.getG());
    //Neuron 1 should be inhibitory
    EXPECT_TRUE(net.neurons[1]->getIsExcitatory());
    //Neuron 99 should be inhibitory
    EXPECT_FALSE(net.neurons[99]->getIsExcitatory());

    //Create a network with g=3, eta=2 and 10 neurons, without backgroundNoise
    Network net2(false, 3,2,100);
    net2.createNetwork();

    //Check G and Vext for one of those neurons
    EXPECT_EQ(3,net2.getG());
    //Neuron 1 should be inhibitory
    EXPECT_TRUE(net2.neurons[1]->getIsExcitatory());
    //Neuron 99 should be inhibitory
    EXPECT_FALSE(net2.neurons[99]->getIsExcitatory());

    
    
}

