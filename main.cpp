#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;


static const float PI = 3.1416;
static const float RADIUS_OF_EARTH_KM = 6371;

struct Coordinates
{
    float longitude;
    float latitude;
    
    Coordinates(float longi, float lati)    
    {
        longitude = longi;
        latitude = lati;
    }
};

struct Defibrillator
{
    Defibrillator()
        :
        coords(0,0)
    {
    }
    
    int id;
    string name;
    string address;
    string phoneNumber;
    Coordinates coords;
};

int findClosestDefibrillatorIdx(vector<Defibrillator>& allDefibData, Coordinates coordB);
float getNumberFromString(string number);
float getDistanceBetweenPoints(Coordinates coordA, Coordinates coordB);
float degreesToRadians(float degrees);

int main()
{
    int const ID_IDX = 0;
    int const NAME_IDX = 1;
    int const ADDRESS_IDX = 2;
    int const PHONE_IDX = 3;
    int const LONGI_IDX = 4;
    
    string LON;
    cin >> LON; cin.ignore();
    string LAT;
    cin >> LAT; cin.ignore();
    int N;
    cin >> N; cin.ignore();
       
    
    // Storage for all defibrillator data
    vector<Defibrillator> allDefibrillatorData;
    
    // Iterate through all defibrillators
    for (int i = 0; i < N; i++) 
    {
        string DEFIB;
        getline(cin, DEFIB);
        
        int index = 0;
        int previousIndex = 0;        
        int fieldIndex = 0;
        
        // Current defibrillator storage        
        Defibrillator defibrillatorData;        
        
        while(index != -1)
        {
            // Get field index in string where each field is separated by ";"
            index = DEFIB.find(";", index);
            
            // If field was actually found
            if(index != -1)
            {
                // Get the current field 
                string field = DEFIB.substr(previousIndex, index-previousIndex); 
                
                // Based on order of field in the string extract the correct value
                switch(fieldIndex)
                {
                    case ID_IDX:
                        defibrillatorData.id = atoi(field.c_str());                        
                    break;
                    
                    case NAME_IDX:
                        defibrillatorData.name = field;                        
                    break;
                    
                    case ADDRESS_IDX:
                        defibrillatorData.address = field;                        
                    break;
                    
                    case PHONE_IDX:
                        defibrillatorData.phoneNumber = field;                        
                    break;
                    
                    case LONGI_IDX:
                        // Convert from string to number and degrees to radians
                        defibrillatorData.coords.longitude = degreesToRadians(getNumberFromString(field));                        
                    break;
                }
             
                
                // Move to next field
                previousIndex = index+1; // +1 to avoid the ";"
                index++;
            }
            else // last field is Latitude
            {                
                // Since last field does not end with ";" it must be retrieved explicitly
                string lastField = DEFIB.substr(previousIndex,
                    DEFIB.length() - previousIndex);          
                    
                // Convert from string to number and degrees to radians
                defibrillatorData.coords.latitude = degreesToRadians(getNumberFromString(lastField));                              
            }
            
            // Move to next field
            fieldIndex++;
        }        
        
        // Add this defibrillator to the list
       allDefibrillatorData.push_back(defibrillatorData); 
    }
    
    // Get user coordinates in numberic format and coverted in radians
    float userLatitude = degreesToRadians(getNumberFromString(LAT));
    float userLongitude = degreesToRadians(getNumberFromString(LON));
    Coordinates userCoords = Coordinates(userLongitude, userLatitude);
    
    // Find the index of closes defibrillator
    int closestDefibrillatorIdx = findClosestDefibrillatorIdx(allDefibrillatorData, userCoords);

    // Output the result     
    cout << allDefibrillatorData[closestDefibrillatorIdx].name << endl;
}


float getNumberFromString(string number)
{
    float resultingNumber = -1;   
    int commaIndex = number.find(",");
    if(commaIndex != -1)
    {
        int lengthToReplace = 1; // Since it is only one character
                   
        // Replace the "," with "."
        number = number.replace(commaIndex, lengthToReplace, "."); 
                   
        // Convert to decimal number
        resultingNumber = atof(number.c_str()); 
    }
    else
    {
        // There is no "," symbol so just convert to decimal number
        resultingNumber = atof(number.c_str()); 
    }
    
    return resultingNumber;
}

// Cauculate distance between two world coordinates
float getDistanceBetweenPoints(Coordinates coordA, Coordinates coordB)
{        
    float x = (coordB.longitude - coordA.longitude) * cos((coordA.latitude + coordB.latitude)/2);
    float y = (coordB.latitude - coordA.latitude);    
    
    return sqrt((x * x) + (y * y)) * RADIUS_OF_EARTH_KM;;
}

float degreesToRadians(float degrees)
{    
    return  degrees * PI/180;
}

int findClosestDefibrillatorIdx(vector<Defibrillator>& allDefibData, Coordinates userCoords)
{
    int closestDefibIndex = -1;
    float closestDistance = std::numeric_limits<int>::max();
    
    // Find the closest defibrillator
    for(int i = 0; i < allDefibData.size(); i++)
    {
        float curDistance = getDistanceBetweenPoints(userCoords, allDefibData[i].coords);
        
        // If a closer defibrillator is found then update the record       
        if(curDistance < closestDistance)
        {
            closestDefibIndex = i;
            closestDistance = curDistance;
        }
    }
    
    return closestDefibIndex;
}