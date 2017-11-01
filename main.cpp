/*
 *  CurrencyConverter
 *      This program reads in a file, and changes numbers
 *      preceded by <TRNAMT> to another, based on the given rate
 *
 *      Arguments:
 *          inFile
 *          outFile
 *          RateOfConversion
 *
 *  Author: Ryland Beaver
 *
 */

#include <iostream>
#include <fstream>
using namespace std;

double grabNum(char* line);
/*
 *      grabNum
 *          grabs the number to be changed
 *          from the line that needs to be changed
 *      arguments:
 *          char* line: The line to read from
 *      return:
 *          returns a double, representing the number to be changed
 */
double strToDbl (char* conv);
/*
 *      strToDbl
 *          Changes a string of a double into a double
 *      return:
 *          returns the double that was converted
 */
void printDbl(fstream& out, double val);
/*
 *      printDbl
 *          prints a double to two decimal places
 *              (If only my compiler recognized strings...)
 */

int main(int argc, char* argv[]) {
    double rate = strToDbl(argv[3]);    //Rate for conversion
    double converted;                   //Amount of money in the transaction
    fstream in(argv[1], ios::in), out(argv[2], ios::out);
                                        //The file to read in
    bool match;                         //Whether the line read in is a line to change
    char testStr[9] = "<TRNAMT>";       //What the line being changed starts with
    char line[70];                      //Stores the line read in

    if(argc != 4) {                     //Check for arguments
        cout << "Incorrect amount of arguments\n";
        cout << "Input, followed by output, followed by rate\n";
        return -1;
    }

    if(in.fail() || out.fail()) {       //Check for file openings
        cout << "Error opening file\n";
        return -2;
    }

    while(in) {                         //Loops through file
        in.getline(line, 399, '\n');
        match = true;

        for(int i = 0; i < 8; i++) {    //Checks for <TRNAMT>
            if(line[i] == '\0') {
                match = false;
                break;
            }
            if(testStr[i] != line[i]) {
                match = false;
            }
        }

        if(match) {
            converted = grabNum(line) / rate;
                                        //Converts money
            out << testStr;
            printDbl(out, converted);
            out << endl;
        } else {
            out << line;
            if (in) {
                out << endl;
            }
        }
    }

    in.close();
    out.close();
    return 0;
}

double grabNum(char* line) {
    char textNum[50];
    int i = 8;
    while(line[i] != '\0') {
        textNum[i-8] = line[i];
        i++;
    }                           //Creates separate string just containing the number
    textNum[i-8] = '\0';
    return strToDbl(textNum);
}

double strToDbl (char* conv) {
    int i = 0;
    double num = 0, division = 0;
    bool isNegative = false;
    if(conv[i] == '-') {
        isNegative = true;
        i++;
    }

    while(conv[i] != '\0') {
        if(conv[i] != '.') {
            if(division == 0) { //If before decimal point, make existing num larger
                num *= 10;
                num += conv[i] - '0';
            } else {
                num += (conv[i] - '0') / division;
                division *= 10; //After, add smaller amounts
            }
        } else {
            division = 10;
        }
        i++;
    }

    if(isNegative) num *= -1;

    return num;
}

void printDbl(fstream& out, double val) {
    if(val < 0) {
        out << '-';
        val *= -1;
    }
    int cast = val; //Grabs as whole num, cuts off decimal
    out << cast << '.';
    cast = (val * 100) - (cast * 100);  //Grabs the decimal
    if (cast < 10) {
        out << '0';
    }
    out << cast;
}