#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include<regex>
using namespace std;
struct Slot {
    int priority;
    mutable bool availability;
    char time;
    int credit;
    string name;
    char type;
    
    bool operator<(const Slot& other) const {
        return std::tie(priority, availability, time, credit, name) <
           std::tie(other.priority, other.availability, other.time, other.credit, other.name);
    }
};

map<Slot, vector<string> > slots;

Slot create_slot(string & slotName){
	Slot newSlot;
    
    newSlot.availability = true;
    if (slotName.find("L") != string::npos) {
        newSlot.type = 'l';
		std::vector<int> numbers;
    	for (size_t i = 0; i < slotName.size(); ++i) {
        	if (std::isdigit(slotName[i])) {
            	int number = 0;
    	        while (i < slotName.size() && std::isdigit(slotName[i])) {
        	        number = number * 10 + (slotName[i] - '0');
            	    ++i;
            	}
            	numbers.push_back(number);
        	}
    	}
    	newSlot.priority = 1;
    	newSlot.time = (numbers.empty() || *std::max_element(numbers.begin(), numbers.end()) <= 30) ? 'm' : 'a'; 
		size_t tCount1 = std::count(slotName.begin(), slotName.end(), '+');
        newSlot.credit = (int)(tCount1 + 1)/2; 
    } else {
        newSlot.type = 't';  
        if (slotName.find("1") != string::npos) {
        	newSlot.time = 'm';  
    	} else if (slotName.find("2") != string::npos) {
        	newSlot.time = 'a';  
    	}
    	regex pattern("[T]+[A-Za-z]+[0-9]+");  
    	if (std::regex_match(slotName, pattern)) {
    		newSlot.credit = 1;
    	} else {
        	size_t tCount = std::count(slotName.begin(), slotName.end(), 'T');
        	newSlot.credit = tCount + 2;
    	}
    	int p;
		cout<<"input priority for "<<slotName<<" :";
		cin>>p;
    	newSlot.priority = p;
    }
	newSlot.name = slotName;
    return newSlot;
}

bool isSlotPresent(map<Slot, vector<string> >& slots, Slot& slot) {
    return slots.find(slot) != slots.end();
}

void addSubjectToSlot(map<Slot, vector<string> >& slots,Slot& slot, string& subject) {
    if (isSlotPresent(slots, slot)) {
        slots[slot].push_back(subject);
    } else {
        slots[slot] = {subject};
    }
}
void find_slots(char time_inp, int credit_inp,char type_inp) {
    map<Slot, vector<string> >::iterator minPriorityIt = slots.end();  
	int min_priority =10;
    
	for (map<Slot, vector<string> >::iterator it = slots.begin(); it != slots.end(); it++) {

    if (it->first.time == time_inp && it->first.credit == credit_inp && it->first.type == type_inp && it->first.availability) {
            
            if ((int)it->first.priority<min_priority) {
                min_priority=(int)it->first.priority;
                minPriorityIt=it;
            }
        }
    }

    if (minPriorityIt != slots.end() ){
        cout << minPriorityIt->first.name;
        minPriorityIt->first.availability = false;
        cout<<endl<<"The available subjects are:"<<endl;
        for (std::vector<string>::iterator it = minPriorityIt->second.begin(); it != minPriorityIt->second.end(); ++it) {
        std::cout << *it <<endl;
    }
    } else {
        cout << "no slot available"<<endl;
    }
}

int main(){
	cout << "Enter m for morning theory or a for afternoon theory:";
    char time_inp;
    cin >> time_inp;
	string filenames[]={"computer_architecture","daa","microprocessors",
	"probability_and_statistics","sts",
	"theory_of_computation","web_programing","daa_lab","microprocessors_lab","probability_and_statistics_lab","web_programing_lab"};
	for (string i:filenames){
		string filename="subject_data/"+i+".csv";
		ifstream file(filename.c_str());
		set<string> uniqueValues;
		string line;
		while(getline(file,line)){
			istringstream linestream(line);
			string firstfield;
			getline(linestream,firstfield,',');
			uniqueValues.insert(firstfield);
		}
		file.close();
	//	cout<<"the slots for "<<i<<" are:"<<endl;
		Slot mySlot;
		for (string value:uniqueValues){
			//cout<<value<<endl;
			if(time_inp=='m'){
				if (value.find("L") != string::npos){
					std::vector<int> numbers;
    				for (size_t i = 0; i < value.size(); ++i) {
        				if (isdigit(value[i])) {
            				int number = 0;
    	        				while (i < value.size() && isdigit(value[i])) {
        	        				number = number * 10 + (value[i] - '0');
            	    				++i;
            					}
            					numbers.push_back(number);
            					if(numbers.empty() || *std::max_element(numbers.begin(), numbers.end()) > 30){
            						mySlot = create_slot(value);
								}
        				}
    				}	
				}
				else{
					if (value.find("1") != string::npos) {
        				mySlot = create_slot(value);
    				} 
				}
			}
			else if(time_inp=='a'){
				if (value.find("L") != string::npos){
					std::vector<int> numbers;
    				for (size_t i = 0; i < value.size(); ++i) {
        				if (isdigit(value[i])) {
            				int number = 0;
    	        				while (i < value.size() && isdigit(value[i])) {
        	        				number = number * 10 + (value[i] - '0');
            	    				++i;
            					}
            					numbers.push_back(number);
            					if(numbers.empty() || *std::max_element(numbers.begin(), numbers.end()) <= 30){
            						mySlot = create_slot(value);
								}
        				}
    				}	
				}
				else{
					if (value.find("2") != string::npos) {
        				mySlot = create_slot(value);
    				} 
				}
			}
			addSubjectToSlot(slots,mySlot,i);
			/*cout << "Priority: " << mySlot.priority << endl;
    		cout << "Availability: " << mySlot.availability << endl;
    		cout << "Time: " << mySlot.time << endl;
    		cout << "Credit: " << mySlot.credit << endl;
    		cout << "Name: " << mySlot.name << endl;*/
    		
		}
	}
	/*for (const auto& entry : slots) {
        const Slot& slot = entry.first;
      	const std::vector<std::string>& subjects = entry.second;

        std::cout << "Slot: " << slot.name << std::endl;
       	std::cout << "Subjects: ";
    	for (const auto& subject : subjects) {
            std::cout << subject << ", ";
        }
        std::cout << std::endl << std::endl;
    }*/
    int total_credit=0;
    int max_credit=27;
    while(max_credit>total_credit){
    	cout<<"theory or lab:(t or l)";
    	char option;
    	cin>>option;
    	if(option=='t'){
    		cout << "Enter credit:";
        	int credit_inp;
        	cin >> credit_inp;
        	find_slots(time_inp, credit_inp,'t');
        	total_credit+=credit_inp;	
		} 
		else if(option=='l'){
			cout << "Enter credit:";
        	int credit_inp;
        	cin >> credit_inp;
        	if(time_inp=='m'){
        		find_slots('a', credit_inp,'l');
        		total_credit+=credit_inp;
			}
			else{
				find_slots('m', credit_inp,'l');
        		total_credit+=credit_inp;
			}
		}
    }
    if(max_credit<=total_credit){
    	cout<<"max credit limit reached";
	}
}

