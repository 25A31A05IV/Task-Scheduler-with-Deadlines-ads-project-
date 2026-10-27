📅 Task Scheduler with Deadlines

An interactive task scheduling application that maximizes total profit by scheduling jobs before their deadlines. Built using C++ and a web-based interface.

🚀 Features

- Add and remove jobs dynamically.
- Greedy scheduling using a Max-Heap.
- Dynamic Programming (DP) profit comparison.
- Display scheduled jobs, total profit, and execution time.
- Interactive and responsive web interface.

🛠️ Tech Stack

- Backend: C++17
- Frontend: HTML, CSS, JavaScript
- Libraries: cpp-httplib, nlohmann/json
- Compiler: GCC/G++

📂 Project Structure

- "Scheduler.cpp" — Greedy scheduling algorithm
- "DP.cpp" — Dynamic Programming implementation
- "server.cpp" — C++ HTTP server
- "main.cpp" — Terminal-based application
- "web/index.html" — Interactive website
- "start_scheduler.bat" — Windows launcher

⚙️ How to Run

Requirements: Windows, GCC/G++, and the "httplib.h" and "json.hpp" library headers.

1. Clone the repository:
   
   git clone https://github.com/25A31A05IV/Task-Scheduler-with-Deadlines-ads-project-.git

2. Open the project folder in VS Code.

3. Double-click "start_scheduler.bat".
   
   Alternatively, compile and run manually:
   
   g++ -std=c++17 server.cpp Scheduler.cpp DP.cpp -o server.exe -lws2_32
.\server.exe

4. Open http://127.0.0.1:8080 in your browser.

🎯 Objective

Demonstrate Greedy Algorithms, Max-Heaps, and Dynamic Programming through a practical scheduling problem.

SDG Alignment: SDG 8 — Decent Work and Economic Growth.

👥 Course Project

Subject: Advanced Data Structures and Algorithm Analysis
