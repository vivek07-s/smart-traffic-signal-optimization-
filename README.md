# Smart Traffic Signal Optimisation

This project models an intelligent traffic control system that adjusts signal timings according to live lane conditions instead of rotating through a fixed timer.

The original codebase is a C++ simulation that:
- tracks four lanes at an intersection
- uses a greedy priority rule to select the next green lane
- adapts green-signal duration based on queue size
- supports morning, evening, midday, and night traffic patterns
- handles emergency-vehicle override and lane blockage scenarios
- compares adaptive signals with fixed-time signals

## Priority Logic

The core decision rule used by the simulator is:

`priority = (vehicle count x average waiting time) / effective capacity`

Emergency vehicles bypass the normal score and receive immediate priority.

## Frontend

A browser dashboard has been added so the project can be explored visually through a local API instead of only through the C++ console flow.

Files:
- `index.html`
- `styles.css`
- `script.js`
- `api_server.py`

The frontend includes:
- scenario switching for the four traffic periods
- a live four-lane intersection view
- greedy lane selection and adaptive green-time display
- emergency override and blocked-lane controls
- a simple greedy vs fixed-time comparison

## How To Run

Start the Python server from the project folder:

`python api_server.py`

Then open:

`http://127.0.0.1:8000`

## Original Source

The original simulation logic remains in the C++ source files such as:
- `main.cpp`
- `simulation.cpp`
- `lane.cpp`
- `trafficSignal.cpp`
