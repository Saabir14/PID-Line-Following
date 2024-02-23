// By Saabir Ahmed Parvaiz Ahmed (2024-2-11)
// All may use this code for any purpose, without any conditions, but at their own risk.
// SensorArray class for reading and mapping sensor values

// Define the SensorArray class
class SensorArray {
  private:
    // Declare private member variables
    int *sensorPins; // Array to hold sensor pin numbers
    int *sensorValues; // Array to hold sensor values
    int pins; // Number of pins

    // Arrays for mapping sensor values
    int *mappedValues;
    int *blackMap;
    int *whiteMap;

    int minMap; // Minimum value for mapping
    int maxMap; // Maximum value for mapping

    int *displacement; // Array to hold sensor displacement values
    
  public:
    // Constructor for SensorArray class
    SensorArray(int *pinsArray, int numPins) {
      pins = numPins; // Set the number of pins
      sensorPins = new int[pins]; // Allocate memory for sensorPins array
      sensorValues = new int[pins]; // Allocate memory for sensorValues array

      // Initialize sensorPins array with values from pinsArray
      for (int i = 0; i < pins; i++) {
        sensorPins[i] = pinsArray[i];
      }

      // Allocate memory for mapping arrays
      mappedValues = new int[pins];
      blackMap = new int[pins];
      whiteMap = new int[pins];

      // Allocate memory for displacement array
      displacement = new int[pins];
    }

    // Destructor for SensorArray class
    ~SensorArray() {
      // Deallocate memory for all arrays
      delete[] sensorPins;
      delete[] sensorValues;
      delete[] mappedValues;
      delete[] blackMap;
      delete[] whiteMap;
    }

    // Method to read sensor values
    void readValues() {
      for (int i = 0; i < pins; i++) {
        sensorValues[i] = analogRead(sensorPins[i]);
      }
    }

    // Method to set mapping values
    void setMap(int *black, int *white, int min = 0, int max = 4095) {
      minMap = min;
      maxMap = max;
      for (int i = 0; i < pins; i++) {
        blackMap[i] = black[i];
        whiteMap[i] = white[i];
      }
    }

    // Method to map sensor values
    void mapValues() {
      for (int i = 0; i < pins; i++) {
        mappedValues[i] = map(sensorValues[i], blackMap[i], whiteMap[i], minMap, maxMap);
        if (mappedValues[i] < minMap) {
          mappedValues[i] = minMap;
        } else if (mappedValues[i] > maxMap) {
          mappedValues[i] = maxMap;
        }
      }
    }

    // Method to set displacement values
    void setDisplacement(int *displacementArray) {
      for (int i = 0; i < pins; i++) {
        displacement[i] = displacementArray[i];
      }
    }

    // Method to calculate weighted average
    int weightedAverage() {
        int sum = 0;
        int total = 0;
        for (int i = 0; i < pins; i++) {
            sum += displacement[i] * mappedValues[i];
            total += abs(displacement[i]);
        }
        if (total == 0) {
            return 0;
        }
        return sum / total;
    }

    // Getter for sensorValues
    const int* getSensorValues() const {
      return sensorValues;
    }

    // Getter for mappedValues
    const int* getMappedValues() const {
      return mappedValues;
    }

    // Method to print sensor values as CSV
    void printCSV() {
      for (int i = 0; i < pins; i++) {
        Serial.print(sensorValues[i]);
        if (i < pins - 1) {
          Serial.print(",");
        }
      }
      Serial.println();
    }

    // Method to print sensor values as TSV
    void printTSV() {
      for (int i = 0; i < pins; i++) {
        Serial.print(sensorValues[i]);
        if (i < pins - 1) {
          Serial.print("\t");
        }
      }
      Serial.println();
    }

    // Method to print mapped sensor values as CSV
    void printMappedCSV() {
      for (int i = 0; i < pins; i++) {
        Serial.print(mappedValues[i]);
        if (i < pins - 1) {
          Serial.print(",");
        }
      }
      Serial.println();
    }

    // Method to print mapped sensor values as TSV
    void printMappedTSV() {
      for (int i = 0; i < pins; i++) {
        Serial.print(mappedValues[i]);
        if (i < pins - 1) {
          Serial.print("\t");
        }
      }
      Serial.println();
    }
};