//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

#include <signal.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>

#include "libpixyusb2.h"

Pixy2        pixy;
static bool  run_flag = true;


using namespace std;

class pattern {
private:
  int signature;
  string name;
public:
    pattern() {
      signature = 00;
      name = "";
    }
    pattern(int signature) {
      int rand = rand() % 4;
      signature = signature;
      /*switch rand {
        case 0:
          name = "Jeff"
        case 1:
          name = "Andrew"
        case 2:
          name = "John"
        case 3:
          name = "Casey"
      }*/
      name = "Jeff";
    }
    string getName() {
      return name;
    }
    int getSignature() {
      return signature;
    }
}



string readlineFile(string filename) {
	string line = "";
	ifstream myfile(filename);
	
	if (myfile.is_open()) {
		getline(myfile,line);
		myfile.close();
	} else {
	    cout << "unable to open";
		cout << filename;
	}
	
	
	return line;
}

void writeResponsetoFile(string filename, string Response) {
	ofstream myfile(filename);
	if (myfile.is_open()) {
		myfile << Response;
		myfile.close();
	} else {
		cout << "unable to open";
		cout << filename;
	}
	
}
string stateOne() {
	  pixy.ccc.getBlocks(128);
	  if (pixy.ccc.numBlocks) {
		pixy.ccc.blocks[0].print(); //current signature is saved to CurrentColorCode.txt in print()
		  return "Yes";  
	  } else {
		  return "NO";
	  }
	  
	  
}
void printSignatureToFile() {
	string age;
	string signature;
	string line;
	int ageLocation;
	
	
	
	line = readlineFile("CurrentColorCode.txt");
	ageLocation= line.find(" ");
	
	age= stoi(line.substr(ageLocation+1));
	signature= line.substr(0 , ageLocation);
	cout << "Age is " << age <<  endl;
	cout << "Siganture is " << signature << endl;
	cout << "age int?" << isdigit(age) << endl; 
	
	if (age > 4) {
		writeResponsetoFile("DetectedObj.txt", signature); //detectedobject's name is printed to DetectedObj.txt
	}
	
  

}

void handle_SIGINT(int unused)
{
  // On CTRL+C - abort! //

  run_flag = false;
}

void  get_blocks()
{
  int  Block_Index;

  // Query Pixy for blocks //
  pixy.ccc.getBlocks(128);

  // Were blocks detected? //
  if (pixy.ccc.numBlocks)
  {
    // Blocks detected - print them! //

    printf ("Detected %d block(s)\n", pixy.ccc.numBlocks);

    for (Block_Index = 0; Block_Index < pixy.ccc.numBlocks; ++Block_Index)
    {
      printf ("  Block %d: ", Block_Index + 1);
      pixy.ccc.blocks[Block_Index].print();
    }
  }
}

int main()
{
  int  Result;
  const char *path="/root/hello.txt";

  // Catch CTRL+C (SIGINT) signals //
  signal (SIGINT, handle_SIGINT);

  printf ("=============================================================\n");
  printf ("= PIXY2 Get Blocks Demo                                     =\n");
  printf ("=============================================================\n");

  printf ("Connecting to Pixy2...");

  // Initialize Pixy2 Connection //
  {
    Result = pixy.init();

    if (Result < 0)
    {
      printf ("Error\n");
      printf ("pixy.init() returned %d\n", Result);
      return Result;
    }

    printf ("Success\n");
  }

  // Get Pixy2 Version information //
  {
    Result = pixy.getVersion();

    if (Result < 0)
    {
      printf ("pixy.getVersion() returned %d\n", Result);
      return Result;
    }

    pixy.version->print();
  }

  // Set Pixy2 to color connected components program //
  pixy.changeProg("color_connected_components");
  printf ("calling cloud speech...");
//system("python3 cloudspeech_demo.py");

  //Init pattern
  pattern patternArray[4];
  pattern pattern1 = pattern();
  pattern pattern2 = pattern();
  pattern pattern3 = pattern();
  pattern pattern4 = pattern();
  patternArray[0] = pattern1;
  patternArray[1] = pattern2;
  patternArray[2] = pattern3;
  patternArray[3] = pattern4;

  while(1)
  {
    //get_blocks();
	writeResponsetoFile("ex.txt", stateOne());
    if (run_flag == false)
    {
      // Exit program loop //
      break;
    }
  }
  printSignatureToFile();

  printf ("PIXY2 Get Blocks Demo Exit\n");
}
