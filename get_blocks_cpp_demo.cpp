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
#include <fstream>
#include <string>

#include "libpixyusb2.h"

Pixy2        pixy;
static bool  run_flag = true;
int patternTracking;


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
    pattern(string givenName, int sig) {
		name = givenName;
		signature = sig;
	}
    pattern(int sig) {
      int ran = rand() % 4;
      signature = sig;
	
      switch (patternTracking) {
        case 0:
          name = "Jeff";
        case 1:
          name = "Andrew";
        case 2:
          name = "John";
        case 3:
          name = "Will";
        case 4: 
         name = "Bob";
         
      }
    }
    string getName() {
      return name;
    }
    int getSignature() {
      return signature;
    }
};

pattern patternArray[4];


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
string running() {
	  pixy.ccc.getBlocks(128);
	  if (pixy.ccc.numBlocks) {  //first line, if there are no blocks, no recongiziton 
		pixy.ccc.blocks[0].print(); //current signature is saved to CurrentColorCode.txt in print()
		  return "Yes";   
	  } else {
		  writeResponsetoFile("CurrentColorCode.txt","0 0");
		  return "NO"; 
	  }
	  
	  
}
int getActivieSignature() {
	int age;
	int signature;
	string line;
	int ageLocation;
	
	
	//run running on a thread and added a mutex lock right here. 
	line = readlineFile("CurrentColorCode.txt");
	ageLocation= line.find(" ");
	
	age= stoi(line.substr(ageLocation+1));
	signature=stoi( line.substr(0 , ageLocation));
	
	return signature;
	//if (age > 4) {
		//pattern cc(signature);	
		//writeResponsetoFile("DetectedObj.txt", cc.getName()); //detectedobject's name is printed to DetectedObj.txt
	//} else {
					
}
string getUserCommand() {
	string command;
	command = readlineFile("voiceCommand.txt");
	return command;
}
bool comeparePatternStateOne(int ActivieSignature) { // pass getActivieSignature() 
	for(int i =0; i <= patternTracking; i++) {
		if(patternArray[i].getSignature() == ActivieSignature){
			string respond = "Yes I recongized " + patternArray[i].getName();
			
			writeResponsetoFile("pixyResponse.txt", respond);//need aa file name to pass detected objects name 
			
			return true;
		} else{
			string response = "No Pattern is not in my database";
			patternArray[patternTracking] = pattern(ActivieSignature);
			
			response+=" but I will save pattern and name it " + patternArray[patternTracking].getName(); 
			writeResponsetoFile("pixyResponse.txt", response);
			patternTracking++;
			
			return false;
		}
	}	 
} 
bool comparePattern(int ActivieSig) {
	for(int i =0; i <= patternTracking; i++) {
		if(patternArray[i].getSignature() == ActivieSig){
			string respond = "Yes I know " + patternArray[i] .getName();
			
			writeResponsetoFile("pixyResponse.txt", respond);//need aa file name to pass detected objects name 
			
			return true;
		} else{
			string response = "No Pattern is not in my database";
			
			writeResponsetoFile("pixyResponse.txt", response);
			
			return false;
		}
	}	
}

void Setpattern(string name, int ActivieSig) {
	patternArray[patternTracking]= pattern(name, ActivieSig);
	
}
void robotSetsName(int ActivieSig) {
	patternArray[patternTracking] = pattern(ActivieSig);
}

void writeToPiPin() {
	system("python3 writeToLed.py"); //led goes high for half a second 
	
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
void sayPixyResponse() {
	system("su pi python3 -c /home/pi/AIY-voice-kit-python/pixy2/build/get_blocks_cpp_demo/sayPixyResponse.py");
}
void userVoiceInput() {
	system("python3 userVoiceInput.py");
}

void handle_SIGINT(int unused)
{
  // On CTRL+C - abort! //

  run_flag = false;
}


int main()
{
  int  Result;

  // Catch CTRL+C (SIGINT) signals //
  signal (SIGINT, handle_SIGINT);

  printf ("=============================================================\n");
  printf ("= PIXY2 Get Blocks Demo                                     =\n");
  printf ("=============================================================\n");

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





  // Set Pixy2 to color connected components program //
  pixy.changeProg("color_connected_components");





  int s;
  bool fl =true;
  
  string cmd = "begin";
  //sayPixyResponse();
  //userVoiceInput();
  while(1)
  {
    running();
    s =getActivieSignature();
    
    
	if ( s > 0 && fl && cmd == "begin") {
		
		comeparePatternStateOne(s);
		cout << "check pixyRespond.txt waiting on you" << endl;
		sayPixyResponse(); 
		cin.get();
		fl =false;
		userVoiceInput();
		cmd =getUserCommand();
	}
	
	
	s=getActivieSignature();
		
	if( fl==false && s > 0 && cmd == "know") {
		s= getActivieSignature();
		comparePattern(s);
		sayPixyResponse();
		cout << "check see if pattern is recongized pixyRespond.txt waiting on you"<< endl;
		cout<< patternArray[0].getName()<< endl;
			
		cin.get();
	}
	
	if(cmd == "turn") {
		writeToPiPin();
		cmd = "";
	}
	
	
	
	
	
    if (run_flag == false)
    {
      // Exit program loop //
      break;
    }
  }

  printf ("PIXY2 Get Blocks Demo Exit\n");
    
}

