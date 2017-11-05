#include "neuron.hpp"
#include "network.hpp"
#include <string>

using namespace std;

/**
 * Function that read all the parameters contained in "../param.in"
 * @return a vector containing the 5 parameters needed
 */
vector<double> readParam();

int main(){
	
    vector<double> param;
    //Vector containing all the parameters useful for the simulation
    try{
        param = readParam();
        
    }
    catch(string errorMsg){
        cerr << errorMsg ;
        return 1;
    }
    
    //Affectation of these parameters into clearer names
    double g(param[0]), eta(param[1]);
    int nbNeurons(param[2]), Start(param[3]), Stop(param[4]);
    
    assert(h>0);
    //We create a network of g, vratio and nb neurons, with backgroundNoise
    Network net(true, g, eta, nbNeurons);
    //The network creates the number of neurons it should contain
    net.createNetwork();

    //Change ms into timesteps
    double Stopstep = static_cast<unsigned long>(ceil(Stop/h));
    //Change ms into timesteps
    double Startstep = static_cast<unsigned long>(ceil(Start/h));
    //The simulation run for the number of steps given
    net.updateNetwork(Startstep, Stopstep);
    
	return 0;

}


vector<double> readParam() {
    
    //Opening of the parameters file
    ifstream read;
    read.open("../param.in");
    //Test if the opening went well
    assert(!read.fail());
    
    //Vector that will contain the parameters
    std::vector<double> param (5, 0);
    
    for(size_t i(0); i < param.size() ; ++i){
        //Reading of all the parameters
        read >> param[i] ;
        
         string error("Invalid argument: ");
        //Test that the parameters are positive numbers
        if( (read.fail() ) or (param[i] < 0) ){
            
            
            if(i==0 or i==1){
                switch (i){
                    case 0:
                        error += "G ";
                        break;
                    case 1:
                        error += "Eta ";
                        break;
        
                        
                }
                error += "must be a positive real number" ;
                
            }
            else{
                switch (i){
                    
                    case 2:
                        error += " The number of neurons ";
                        break;
                    case 3:
                        error += " The start time ";
                        break;
                    case 4:
                        error +=  " The stop time ";
                        break;
                    
                }
                error +=  " must be a positive integer number";
            }
            
           throw(error);
        }
        //Test that the number of neurons is more than 50
        else if (i==2 and param[i] < 50){
            cout << param[3];
            error += " Not enough neurons in the simulation for the chosen ration 0.8:0.2 excitatory:inhibitory. At least 50. " ;
          throw(error);
            
        }
        //Test that the start time is smaller than the stop time
        else if (i==4 and param[4] < param[3]){
            error += " The stop time must be bigger than the start time. ";
            throw(error);
        }
        
    }
    //Closing of the flow
    read.close();
    
    return param;
}


