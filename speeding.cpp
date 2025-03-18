#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

//Array of three letter months in order to convert month number to a 3 letter abbreivation
const string THREE_LETTER_MONTHS[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};

//Constant ticket multipliers for calculating the ticket amount
const double INTERSTATE_MULTIPLIER = 5.2243;
const double HIGHWAY_MULTIPLIER = 9.4312;
const double RESIDENTIAL_MULTIPLIER = 17.2537;
const double DEFAULT_MULTIPLIER = 12.916;

//The serializeDate method turns the inputted year, month, and day into a serialDate
int serializeDate(const int year, const int month, const int day) {
    int serialDate = (year * 10000) + (month * 100) + day;
    return serialDate;
}

//calcSpeedTicket Method uses the road type, speed limit, and clocked speed in order to calculate the speeding ticket amount 
//If the fine is negative, then a ticketCost of $0 is returned
double calcSpeedTicket(char rType, int sLimit, int clocSpeed) {
    double ticketCost; 
    int speedDiff = clocSpeed - sLimit;
    switch (rType) {
        case 'i':
        case 'I':
            ticketCost = speedDiff * INTERSTATE_MULTIPLIER; 
            break;
        case 'h': 
        case 'H': 
            ticketCost = speedDiff * HIGHWAY_MULTIPLIER; 
            break;
        case 'r':
        case 'R': 
            ticketCost = speedDiff * RESIDENTIAL_MULTIPLIER; 
            break;
        default: 
            ticketCost = speedDiff * DEFAULT_MULTIPLIER; 
            break;
    }

    if (ticketCost < 0.00) {
        ticketCost = 0.00;
    }


return ticketCost; 
}

//The check year method checks for two digit years and converts them to a four digit year if necessary
int yearCheck(int yearInitial) {
    int checkedYear = yearInitial;
    if (yearInitial < 100) {
        checkedYear = yearInitial + 2000;
    } 
return checkedYear;
}


int main() {
    //All necessary variables are declared
    //The fin and fout streams are declared 
    ifstream fin;
    ofstream fout;

    string inputFile;
    string outputFile;
    int startDay, startMonth, startYear, checkedStartYear;
    int endDay, endMonth, endYear, checkedEndYear;
    
    string citNum;
    char roadType;
    int month, day, year, clockedSpeed, speedLim;
    int serializedStart;
    int serializedEnd;
    double ticket; 

    //Prompts for ticket file and report file. Checks if both the files are valid
    cout << "Enter a ticket file: ";
    cin >> inputFile;

    fin.open(inputFile);
    if (!fin.is_open()) {
        cerr << "Unable to open " << inputFile << "." << endl;
        return 1;
    }

    cout << "Enter a report file: ";
    cin >> outputFile;
    
    fout.open(outputFile);
    if (!fout.is_open()) {
        cerr << "Unable to open " << outputFile << "." << endl;
        return 1; 
    }

    //Prompts user to enter the start and end dates
    cout << "Enter report start date (mm dd yyyy): ";
    cin >> startMonth >> startDay >> startYear; 
    cout << "Enter report end date" << setw(17) << setfill(' ') << right << "(mm dd yyyy): ";
    cin >> endMonth >> endDay >> endYear; 

    //Calls yearCheck methods to check for two digits years and then serializes the start and end dates
    checkedStartYear = yearCheck(startYear);
    checkedEndYear = yearCheck(endYear);
    serializedStart = serializeDate(checkedStartYear, startMonth, startDay);
    serializedEnd = serializeDate(checkedEndYear, endMonth, endDay); 
    
    //While loop reads in all details of the input/ticket file
    while (fin >> citNum >> month >> day >> year >> clockedSpeed >> speedLim >> roadType) {
        //Checks for a two digit year and serializes the ticket file dates
        int checkedYear = yearCheck(year);
        int currentRange = serializeDate(checkedYear, month, day);

        //Checks if the serialized date of the ticket file falls within the range of serialized dates inputted by the user
        if (currentRange >= serializedStart && currentRange <= serializedEnd) {
            //Converts the number month within the ticket files to a 3 letter month
            string currentMonth = THREE_LETTER_MONTHS[month - 1];
            //Calculates the speeding ticket amount
            ticket = calcSpeedTicket(roadType, speedLim, clockedSpeed);
            //Formats and writes to the output file 
            fout << setw(2) << setfill('0') << day << '-' << currentMonth << '-' << checkedYear;
            fout << " " << setw(11) << setfill(' ') << left << citNum << '$';
            fout << setw(9) << right << setfill(' ') << setprecision(2) << fixed << ticket << endl; 
        } else {
            continue;
        }
    }
    //Closes the fin and fout streams 
    fin.close();
    fout.close();

    return 0;
}
