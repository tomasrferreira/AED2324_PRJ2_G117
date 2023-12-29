//
// Created by Tomás Ferreira on 28/12/2023.
//

#include <iostream>
#include "src/FlightManager.h"

void showMenu();

void bestTrajectory(FlightManager& h);

void showBestTrajectoryMenu();

void showAirportInfoMenu();

void askForAirport(FlightManager& h);

using namespace std;
int main() {
    FlightManager h;
    h.lerFicheiros();
    bool keepRunning = true;
    while(keepRunning){
        showMenu();
        int option;
        cin >> option;
        if (cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            option = 0;
        }
        switch (option){
            case 1:
                h.startingPoint();
                break;
            case 2:
                h.askForAirport();
                break;
            case 3:
                h.askForOtherInfo();
                break;
            case 4:
                cout << "\nGOOD FLIGHT! ENJOY YOUR TRIP!" << endl;
                keepRunning = false;
                break;
            default:
                cout << "!!Invalid Input!!\n\n";
        }
    }
}


//-----------------------Menus-----------------------------------

void showMenu(){
    cout << "=========================\n";
    cout << "| Menu :                |\n";
    cout << "| 1- Best Trajectory    |\n";
    cout << "| 2- Airport Info       |\n";
    cout << "| 3- Other Info         |\n";
    cout << "| 4- Quit               |\n";
    cout << "=========================\n";
    cout << "Pick an option:";
}