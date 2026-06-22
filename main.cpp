#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Final object used by the prioritization engine.
struct DetectedObject
{
    int id;
    string objectName;
    float distance;
    float speed;
    int riskScore;
};

// Simulated readings from the autonomous vehicle sensor architecture.
struct SensorReading
{
    int id;
    string cameraObjectName;
    float lidarDistance;
    float radarSpeed;
    string gpsLocation;
    string direction;
};

string toLowerCase(string text)
{
    for (char& character : text)
    {
        character = static_cast<char>(tolower(static_cast<unsigned char>(character)));
    }

    return text;
}

float readNonNegativeFloat(const string& message)
{
    float value;

    while (true)
    {
        cout << message;
        cin >> value;

        if (!cin.fail() && value >= 0)
        {
            return value;
        }

        cout << "Invalid input. Value cannot be negative.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readDirection()
{
    string direction;

    while (true)
    {
        cout << "Enter direction (approaching, crossing, moving_away): ";
        cin >> direction;
        direction = toLowerCase(direction);

        if (direction == "approaching" || direction == "crossing" || direction == "moving_away")
        {
            return direction;
        }

        cout << "Invalid direction. Use approaching, crossing, or moving_away.\n";
    }
}

int getObjectRisk(const string& objectName)
{
    string name = toLowerCase(objectName);

    if (name == "pedestrian") return 30;
    if (name == "dog" || name == "animal") return 25;
    if (name == "bicycle" || name == "motorcycle") return 22;
    if (name == "vehicle" || name == "car") return 18;
    if (name == "truck" || name == "bus") return 15;
    return 10;
}

int getDirectionRisk(const string& direction)
{
    if (direction == "crossing") return 15;
    if (direction == "approaching") return 10;
    return 0;
}

int calculateCollisionProbability(const SensorReading& reading)
{
    int proximityScore = reading.lidarDistance <= 5 ? 55
                       : reading.lidarDistance <= 10 ? 40
                       : reading.lidarDistance <= 20 ? 25
                       : 10;

    int speedScore = reading.radarSpeed >= 70 ? 30
                   : reading.radarSpeed >= 40 ? 20
                   : reading.radarSpeed >= 15 ? 10
                   : 5;

    int probability = proximityScore + speedScore + getDirectionRisk(reading.direction);
    return probability > 100 ? 100 : probability;
}

int calculateRiskScore(const SensorReading& reading)
{
    int distanceRisk = reading.lidarDistance <= 5 ? 35
                     : reading.lidarDistance <= 10 ? 28
                     : reading.lidarDistance <= 20 ? 20
                     : reading.lidarDistance <= 35 ? 10
                     : 5;

    int speedRisk = reading.radarSpeed >= 70 ? 20
                  : reading.radarSpeed >= 40 ? 15
                  : reading.radarSpeed >= 15 ? 8
                  : 3;

    int collisionRisk = calculateCollisionProbability(reading) >= 80 ? 20
                      : calculateCollisionProbability(reading) >= 50 ? 12
                      : 5;

    int riskScore = distanceRisk + speedRisk + getObjectRisk(reading.cameraObjectName)
                  + getDirectionRisk(reading.direction) + collisionRisk;

    return riskScore > 100 ? 100 : riskScore;
}

DetectedObject fuseSensorReading(const SensorReading& reading)
{
    return {reading.id, reading.cameraObjectName, reading.lidarDistance,
            reading.radarSpeed, calculateRiskScore(reading)};
}

void sensorFusionEngine(const vector<SensorReading>& readings, vector<DetectedObject>& objects)
{
    objects.clear();

    for (const SensorReading& reading : readings)
    {
        objects.push_back(fuseSensorReading(reading));
    }
}

void loadSampleSensorData(vector<SensorReading>& readings, vector<DetectedObject>& objects)
{
    readings.clear();

    readings.push_back({1, "Truck", 35.0f, 70.0f, "Highway_A", "approaching"});
    readings.push_back({2, "Pedestrian", 8.0f, 5.0f, "Highway_A", "crossing"});
    readings.push_back({3, "Bicycle", 12.0f, 15.0f, "Highway_A", "approaching"});
    readings.push_back({4, "Vehicle", 20.0f, 60.0f, "Highway_A", "approaching"});
    readings.push_back({5, "Dog", 6.0f, 4.0f, "Highway_A", "crossing"});

    sensorFusionEngine(readings, objects);
}

void captureSensorReading(vector<SensorReading>& readings, vector<DetectedObject>& objects)
{
    SensorReading reading;
    reading.id = static_cast<int>(readings.size()) + 1;

    cout << "\nCamera detected object name: ";
    cin >> reading.cameraObjectName;
    reading.lidarDistance = readNonNegativeFloat("LiDAR distance in meters: ");
    reading.radarSpeed = readNonNegativeFloat("Radar relative speed in km/h: ");
    cout << "GPS location label: ";
    cin >> reading.gpsLocation;
    reading.direction = readDirection();

    readings.push_back(reading);
    sensorFusionEngine(readings, objects);
    cout << "\nSensor reading captured, fused, and risk assessed successfully.\n";
}

void displaySensorReadings(const vector<SensorReading>& readings)
{
    if (readings.empty())
    {
        cout << "\nNo sensor readings available.\n";
        return;
    }

    cout << "\nSensor Layer: LiDAR + Radar + Camera + GPS\n";
    cout << left << setw(6) << "ID"
         << setw(16) << "Camera Object"
         << setw(14) << "LiDAR(m)"
         << setw(16) << "Radar(km/h)"
         << setw(16) << "GPS"
         << setw(16) << "Direction"
         << setw(14) << "Collision %" << "\n";
    cout << string(98, '-') << "\n";

    for (const SensorReading& reading : readings)
    {
        cout << left << setw(6) << reading.id
             << setw(16) << reading.cameraObjectName
             << setw(14) << fixed << setprecision(1) << reading.lidarDistance
             << setw(16) << fixed << setprecision(1) << reading.radarSpeed
             << setw(16) << reading.gpsLocation
             << setw(16) << reading.direction
             << setw(14) << calculateCollisionProbability(reading) << "\n";
    }
}

void displayObjects(const vector<DetectedObject>& objects)
{
    if (objects.empty())
    {
        cout << "\nNo fused objects available. Load or capture sensor data first.\n";
        return;
    }

    cout << "\nSensor Fusion and Risk Assessment Output\n";
    cout << left << setw(8) << "ID"
         << setw(18) << "Object"
         << setw(16) << "Distance(m)"
         << setw(14) << "Speed"
         << setw(12) << "Risk Score" << "\n";
    cout << string(68, '-') << "\n";

    for (const DetectedObject& object : objects)
    {
        cout << left << setw(8) << object.id
             << setw(18) << object.objectName
             << setw(16) << fixed << setprecision(1) << object.distance
             << setw(14) << fixed << setprecision(1) << object.speed
             << setw(12) << object.riskScore << "\n";
    }
}

int partition(vector<DetectedObject>& objects, int low, int high)
{
    int pivot = objects[high].riskScore;
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (objects[j].riskScore > pivot)
        {
            i++;
            swap(objects[i], objects[j]);
        }
    }

    swap(objects[i + 1], objects[high]);
    return i + 1;
}

void quickSort(vector<DetectedObject>& objects, int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(objects, low, high);
        quickSort(objects, low, pivotIndex - 1);
        quickSort(objects, pivotIndex + 1, high);
    }
}

void showPriorityRanking(vector<DetectedObject>& objects)
{
    if (objects.empty())
    {
        cout << "\nNo objects available for priority ranking.\n";
        return;
    }

    quickSort(objects, 0, static_cast<int>(objects.size()) - 1);
    cout << "\nQuick Sort Prioritization Engine\n";
    cout << string(48, '-') << "\n";

    for (int i = 0; i < static_cast<int>(objects.size()); i++)
    {
        cout << "Priority " << i + 1 << " -> "
             << objects[i].objectName << " -> Risk Score: "
             << objects[i].riskScore << "\n";
    }
}

void decisionControlSystem(vector<DetectedObject>& objects)
{
    if (objects.empty())
    {
        cout << "\nNo object is available for decision control.\n";
        return;
    }

    quickSort(objects, 0, static_cast<int>(objects.size()) - 1);
    const DetectedObject& highestRiskObject = objects[0];

    cout << "\nDecision Control System\n";
    cout << "Highest Priority Object: " << highestRiskObject.objectName << "\n";
    cout << "Risk Score: " << highestRiskObject.riskScore << "\n";
    cout << "Vehicle Actuator Decision: ";

    if (highestRiskObject.riskScore >= 85)
    {
        cout << "Apply emergency braking and activate collision warning.\n";
    }
    else if (highestRiskObject.riskScore >= 65)
    {
        cout << "Reduce speed and evaluate steering or lane-change alternatives.\n";
    }
    else if (highestRiskObject.riskScore >= 40)
    {
        cout << "Maintain safe distance and monitor the object closely.\n";
    }
    else
    {
        cout << "Continue normal driving while monitoring surroundings.\n";
    }
}

void showArchitectureStatus()
{
    cout << "\nAutonomous Vehicle Architecture Simulation\n";
    cout << "1. LiDAR Module: provides object distance.\n";
    cout << "2. Radar Module: provides relative object speed.\n";
    cout << "3. Camera Module: identifies the object type.\n";
    cout << "4. GPS Module: provides the location label.\n";
    cout << "5. Sensor Fusion Engine: combines all sensor readings.\n";
    cout << "6. Risk Assessment Module: calculates a risk score automatically.\n";
    cout << "7. Quick Sort Engine: ranks objects by descending risk score.\n";
    cout << "8. Decision Control System: recommends a vehicle action.\n";
}

void runPrioritySystem()
{
    vector<SensorReading> readings;
    vector<DetectedObject> objects;
    int choice;

    do
    {
        cout << "\nAutonomous Vehicle Sensor Data Prioritization System\n";
        cout << "1. Load sample sensor data\n";
        cout << "2. Capture sensor reading\n";
        cout << "3. Display sensor readings\n";
        cout << "4. Run Sensor Fusion and Risk Assessment\n";
        cout << "5. Display fused detected objects\n";
        cout << "6. Sort objects by risk using Quick Sort\n";
        cout << "7. Show Priority Ranking\n";
        cout << "8. Run Decision Control System\n";
        cout << "9. Show architecture modules\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cout << "\nInvalid choice. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
            continue;
        }

        switch (choice)
        {
        case 1:
            loadSampleSensorData(readings, objects);
            cout << "\nSample data loaded through LiDAR, Radar, Camera, and GPS modules.\n";
            break;
        case 2:
            captureSensorReading(readings, objects);
            break;
        case 3:
            displaySensorReadings(readings);
            break;
        case 4:
            sensorFusionEngine(readings, objects);
            cout << "\nSensor Fusion Engine and Risk Assessment Module completed.\n";
            break;
        case 5:
            displayObjects(objects);
            break;
        case 6:
            if (objects.empty())
            {
                cout << "\nNo objects available to sort.\n";
            }
            else
            {
                quickSort(objects, 0, static_cast<int>(objects.size()) - 1);
                cout << "\nObjects sorted by risk score using Quick Sort.\n";
                displayObjects(objects);
            }
            break;
        case 7:
            showPriorityRanking(objects);
            break;
        case 8:
            decisionControlSystem(objects);
            break;
        case 9:
            showArchitectureStatus();
            break;
        case 10:
            cout << "\nExiting system.\n";
            break;
        default:
            cout << "\nInvalid choice. Please try again.\n";
        }
    } while (choice != 10);
}

int main()
{
    runPrioritySystem();
    return 0;
}
