#include "neuron.hpp"
#include "network.hpp"

using namespace std;

/**
 * Function that read all the parameters contained in "../param.in"
 * @return a vector containing the 5 parameters needed
 */
vector<double> readParam();

int main(){
	
    //Vector containing all the parameters useful for the simulation
    vector<double> param = readParam();
    
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
        
        //Test that the parameters are positive numbers
        if( (read.fail() ) or (param[i] < 0) ){
            
            cerr << "Invalid argument:";
            
            if(i==0 or i==1){
                switch (i){
                    case 0:
                        cerr << "G ";
                        break;
                    case 1:
                        cerr << "Eta ";
                        break;
        
                        
                }
                cerr << "must be a positive real number" << endl;
                
            }
            else{
                switch (i){
                    
                    case 2:
                        cerr << "The number of neurons ";
                        break;
                    case 3:
                        cerr << "The start time ";
                        break;
                    case 4:
                        cerr << "The stop time ";
                        break;
                    
                }
                cerr << "must be a positive integer number";
            }
            
            throw invalid_argument(" ");
        }
        //Test that the number of neurons is more than 50
        else if (i==2 and param[i] < 50){
            cout << param[3];
            cerr << " Not enough neurons in the simulation for the chosen ration 0.8:0.2 excitatory:inhibitory. At least 50. " << endl;
            throw invalid_argument(" ");
            
        }
        //Test that the start time is smaller than the stop time
        else if (i==4 and param[4] < param[3]){
            cerr << "The stop time must be bigger than the start time. " << endl;
            throw invalid_argument(" ");
        }
      
    }
    //Closing of the flow
    read.close();
    
    return param;
}


