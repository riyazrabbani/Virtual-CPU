# CPU Register Simulation

This project simulates five CPU registers (A-E), each holding a dynamic list of integers. It uses **C++** to simulate the CPU registers and populate them with random values at regular intervals. The updates are displayed in the console every second.

### CPU Register Simulation
- CPU Registers Simulation: Simulates 5 CPU registers (A, B, C, D, E), each holding a random list of integers.
- Random Number Generation: Random integers are added to the registers at regular intervals.

## To Run the CPU
- g++ -std=c++20 -pthread app.cpp -o app
- Use the above command to run, cpp 20, pthread library linkage for multithreading

## To view in GraphQL
- uvicorn graphql_server:app --reload
- Must have uvicorn, graphql, fastAPI installed

### Web API and Data Integration
- **GraphQL**: Will be used to fetch register data.
- **Strawberry**: GraphQL library for Python, integrating with FastAPI for the API layer.
- **PostgreSQL**: Backend data store for all updates.
