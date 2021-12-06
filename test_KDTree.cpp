//
//  test_KDTree.cpp
//
//
//  Created by Sivaram Ambikasaran on 12/3/13.
//
//

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "../header/KDTree.hpp"

void display(std::string display_String, unsigned n_Locations, unsigned n_Dimension, double* locations, unsigned n_Properties, double* properties) {
        std::cout << display_String << std::endl;

        unsigned count_Location  =       0;
        unsigned count_Property  =       0;

        for (unsigned j=0; j<n_Locations; ++j) {
                for (unsigned k=0; k<n_Dimension; ++k) {
                        std::cout << locations[count_Location] << "\t";
                        ++count_Location;
                }
                for (unsigned k=0; k<n_Properties; ++k) {
                        std::cout << properties[count_Property] << "\t";
                        ++count_Property;
                }
                std::cout << std::endl;
        }
        std::cout << std::endl;
}

int main(int argc, char** argv) {
        assert(argc==4);
        unsigned n_Locations    =       atoi(argv[1]);  //      Number of points.
        unsigned n_Dimension    =       atoi(argv[2]);  //      Dimension.
        unsigned MinParticlesInLeaf   =       atoi(argv[3]);  //      Number of properties.
        unsigned n_Properties   =       0;  //      Number of properties.

        unsigned nLevels = log(n_Locations/MinParticlesInLeaf)/log(4);

        double* locations       =       new double [n_Locations*n_Dimension];   //      Stores all the locations.
        double* properties      =       new double [n_Locations*n_Properties];  //      Stores all the properties.

        //      Generates random locations and random values of property.
        unsigned count_Location =       0;
        unsigned count_Property =       0;
        for (unsigned j=0; j<n_Locations; ++j) {
                for (unsigned k=0; k<n_Dimension; ++k) {
                        locations[count_Location]       =       2*double(rand())/double(RAND_MAX)-1;//2*(int(rand())%2)-1;//double(rand())/double(RAND_MAX);
                        ++count_Location;
                }
                for (unsigned k=0; k<n_Properties; ++k) {
                        properties[count_Property]      =       double(rand())/double(RAND_MAX);
                        ++count_Property;
                }
        }

        std::cout << std::scientific;

        //      Display the initial contents.
       display("Initial contents: ", n_Locations, n_Dimension, locations, n_Properties, properties);

        //      Creates a KDTree given the locations.
        KDTree* A                       =       new KDTree(n_Locations, n_Dimension, locations, n_Properties, properties, MinParticlesInLeaf, nLevels);
        delete locations;
        delete properties;

        double* sorted_Locations        =       new double[n_Locations*n_Dimension];
        double* sorted_Properties       =       new double[n_Locations*n_Properties];

        std::vector<int> NumberOfParticlesInLeaves;//vec that contains number of particels in each leaf
        NumberOfParticlesInLeaves.reserve(pow(4,nLevels));

        clock_t start, end;
        //      Sorts the locations based on KDTree.
        start   =       clock();
        int temp = A->sort_KDTree(NumberOfParticlesInLeaves);
        end     =       clock();

        // std::cout << "size: " << NumberOfParticlesInLeaves.size() << std::endl;
        // for (size_t i = 0; i < NumberOfParticlesInLeaves.size(); i++) {
        //   std::cout << NumberOfParticlesInLeaves[i] << std::endl;
        //   if((i+1)%4 == 0) {
        //     std::cout << "-----------------" << std::endl;
        //   }
        // }

        // converting N ordering to the mirrored C ordering.
        // The sorted locations that the KD Tree outputs, correspond to boxes in N ordering.
        // So here we are generating N ordering sequence of box numbers in terms of the mirrored C ordering.
        // only leaf level ordering is what we want: boxNumbers[nLevels]
        std::vector<std::vector<int> > boxNumbers;
        std::vector<int> boxNumbersInLevel0;
        boxNumbersInLevel0.push_back(0);//level 0
        boxNumbers.push_back(boxNumbersInLevel0);
        for (size_t j = 1; j <= nLevels; j++) {
          std::vector<int> boxNumbersInALevel;
          for (size_t k = 0; k < boxNumbers[j-1].size(); k++) {
            boxNumbersInALevel.push_back(boxNumbers[j-1][k]*4 + 0);
            boxNumbersInALevel.push_back(boxNumbers[j-1][k]*4 + 3);
            boxNumbersInALevel.push_back(boxNumbers[j-1][k]*4 + 1);
            boxNumbersInALevel.push_back(boxNumbers[j-1][k]*4 + 2);
          }
          boxNumbers.push_back(boxNumbersInALevel);
        }

        // for (size_t k = 0; k < boxNumbers[nLevels].size(); k++) {
        //   std::cout << boxNumbers[nLevels][k] << ", ";
        // }
        // std::cout << std::endl;

        // int nLevels = 0;
        // while(1) {
        //   if (temp >= pow(4,nLevels)) {
        //     temp -= pow(4,nLevels);
        //     ++nLevels;
        //   }
        //   else {
        //     break;
        //   }
        // }


        std::cout << "Time taken is: " << double(end-start)/double(CLOCKS_PER_SEC) << std::endl;
        std::cout << "Number of Levels of tree: " << nLevels << std::endl;
        //      Obtains the sorted location.
        // sorted_Locations contains the locations sorted as per the KD Tree.(In N ordering of boxes)
        A->get_Location_Properties(sorted_Locations, sorted_Properties);

        //      Display the sorted contents.
       // display("Sorted contents: ", n_Locations, n_Dimension, sorted_Locations, n_Properties, sorted_Properties);

        delete sorted_Locations;
        delete sorted_Properties;
        delete A;
}
