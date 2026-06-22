# Autonomous Vehicle Sensor Data Prioritization System

This is a Semester-II C++ Data Structures and Algorithms project that simulates how an autonomous vehicle can prioritize surrounding objects. It is a console simulation: it does not connect to physical vehicle hardware or real sensors.

## Project Objective

The project collects simulated LiDAR, Radar, Camera, and GPS readings, combines them through a Sensor Fusion Engine, calculates a risk score, ranks objects with manual Quick Sort, and sends the highest-priority object to a Decision Control System.

## Autonomous Vehicle Architecture

| Component | Simulated Responsibility |
| --- | --- |
| LiDAR | Measures the distance of an object from the vehicle. |
| Radar | Measures the relative speed of an object. |
| Camera | Identifies the object type, such as Pedestrian or Truck. |
| GPS | Stores the vehicle or road location label. |
| Sensor Fusion Engine | Combines the readings into a `DetectedObject`. |
| Risk Assessment Module | Calculates a risk score from distance, speed, object type, direction, and collision probability. |
| Quick Sort Prioritization Engine | Sorts objects from highest risk score to lowest. |
| Decision Control System | Recommends braking, speed reduction, steering/lane evaluation, or monitoring. |

## Project Structure

```text
Autonomous-Vehicle-Prioritization/
├── README.md
├── main.cpp
└── data/
    └── sample_objects.txt
```

## System Workflow

```text
LiDAR + Radar + Camera + GPS
              |
              v
      Sensor Fusion Engine
              |
              v
     Risk Assessment Module
              |
              v
  Manual Quick Sort Prioritization
              |
              v
      Priority Ranking Module
              |
              v
      Decision Control System
```

## Data Structures

`SensorReading` represents the raw readings collected by the simulated sensor layer.

```cpp
struct SensorReading
{
    int id;
    string cameraObjectName;
    float lidarDistance;
    float radarSpeed;
    string gpsLocation;
    string direction;
};
```

`DetectedObject` is the fused output that Quick Sort prioritizes.

```cpp
struct DetectedObject
{
    int id;
    string objectName;
    float distance;
    float speed;
    int riskScore;
};
```

## Risk Assessment Module

The risk score is calculated automatically; the user does not enter it directly. The calculation considers:

- LiDAR distance: nearer objects receive more risk points.
- Radar speed: faster objects receive more risk points.
- Camera object type: pedestrians, animals, bicycles, and motorcycles receive higher risk points.
- Direction: crossing and approaching objects are considered more dangerous.
- Predicted collision probability: calculated from distance, speed, and direction.

The final score is limited to the range `0` to `100`.

## Quick Sort Explanation

Quick Sort is manually implemented in `partition()` and `quickSort()`. `std::sort()` is not used.

The last object is used as the pivot. This comparison sorts in descending priority order:

```cpp
objects[j].riskScore > pivot
```

Therefore, the object with the highest risk score is placed first.

## Complexity Analysis

| Analysis | Complexity |
| --- | --- |
| Quick Sort best case | `O(n log n)` |
| Quick Sort average case | `O(n log n)` |
| Quick Sort worst case | `O(n^2)` |
| Quick Sort average recursive stack space | `O(log n)` |

Quick Sort is suitable because it is fast on average and performs in-place partitioning, which keeps additional memory use low.

## Requirement Checklist

- ✓ LiDAR distance module simulated.
- ✓ Radar speed module simulated.
- ✓ Camera object-identification module simulated.
- ✓ GPS location module simulated.
- ✓ Sensor Fusion Engine implemented.
- ✓ Risk Assessment Module calculates risk automatically.
- ✓ Collision probability is estimated from sensor values.
- ✓ Manual Quick Sort implemented.
- ✓ `std::sort()` not used.
- ✓ Objects sorted in descending order of risk score.
- ✓ Priority Ranking Module implemented.
- ✓ Decision Control System implemented.
- ✓ Input validation prevents negative distance and speed.
- ✓ Single-file C++ project maintained in `main.cpp`.

## Case Study Requirement Verification

| Requirement | Status | Verification |
| --- | --- | --- |
| LiDAR | Implemented as simulation | `lidarDistance` is captured and used in risk calculation. |
| Radar | Implemented as simulation | `radarSpeed` is captured and used in risk calculation. |
| Camera | Implemented as simulation | `cameraObjectName` identifies the object type. |
| GPS | Implemented as simulation | `gpsLocation` is captured and displayed. |
| Sensor Fusion Engine | Implemented | Combines sensor readings into `DetectedObject`. |
| Risk Assessment Module | Implemented | Calculates risk from distance, speed, type, direction, and collision probability. |
| Quick Sort Prioritization Engine | Implemented | Manually ranks fused objects in descending risk order. |
| Decision Control System | Implemented | Recommends braking, speed reduction, steering/lane evaluation, or monitoring. |
| Vehicle actuators | Simulated | Decisions are printed as console actions; no real vehicle hardware is controlled. |
| Real-time processing | Simulated | Each menu action runs the full pipeline on the latest captured readings. |

## Compile And Run

```bash
g++ -std=c++17 main.cpp -o vehicle_priority
./vehicle_priority
```
