#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <string>
#include <map>
#include <fstream>
#include <mutex>

class CPURegisters {
public:
    CPURegisters() {
        // Register creation
        registers = {{"A", {}}, {"B", {}}, {"C", {}}, {"D", {}}, {"E", {}}};
    }

    void add_random_value() {
        // Randomly choose a register, using uniform dist 1 - 100
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(1, 100);

        // Storing keys with indexes
        std::vector<std::string> register_keys = {"A", "B", "C", "D", "E"};
        std::uniform_int_distribution<> reg_dist(0, 4);

        // Choose register and generate a random value
        std::string chosen_register = register_keys[reg_dist(gen)];
        int value = dist(gen);

        // Lock while modifying registers
        std::lock_guard<std::mutex> lock(mutex);
        
        // Push to the top of the list
        registers[chosen_register].insert(registers[chosen_register].begin(), value);
        
        // If the size of the vector exceeds 10, remove the last element
        if (registers[chosen_register].size() > 10) {
            registers[chosen_register].pop_back();
        }
    }

    void display_registers() {
        // Lock while reading registers for display
        std::lock_guard<std::mutex> lock(mutex);
        
        // Use a temporary file to avoid race conditions
        std::ofstream temp_file("cpu_registers_temp.txt");
        
        if (!temp_file.is_open()) {
            std::cerr << "Error opening temporary file!" << std::endl;
            return;
        }

        // Iterate through all the registers and write to temp file
        for (auto& reg : registers) {
            temp_file << "Register " << reg.first << ": [ ";
            for (int val : reg.second) {
                temp_file << val << " ";
            }
            temp_file << "] Length: " << reg.second.size() << "\n";
        }
        
        // Close the temp file
        temp_file.close();
        
        // Atomically replace the output file with the temp file
        #ifdef _WIN32
        std::remove("cpu_registers_output.txt");
        std::rename("cpu_registers_temp.txt", "cpu_registers_output.txt");
        #else
        std::rename("cpu_registers_temp.txt", "cpu_registers_output.txt");
        #endif
    }

private:
    std::map<std::string, std::vector<int>> registers;
    std::mutex mutex;  // To protect concurrent access to registers
};

void update_registers(CPURegisters& cpu) {
    while (true) {
        cpu.add_random_value();
        cpu.display_registers();
        
        // Less frequent updates to reduce file I/O load
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Don't clear the console - this makes debugging easier
        // system("clear");  // Commented out
    }
}

int main() {
    CPURegisters cpu;
    std::cout << "Simulating real-time register updates...\n" << std::endl;
    std::cout << "Register data is being written to cpu_registers_output.txt\n" << std::endl;
    
    std::thread update_thread(update_registers, std::ref(cpu));
    
    // Keep the program running until user presses Ctrl+C
    std::cout << "Press Ctrl+C to exit...\n" << std::endl;
    update_thread.join();
    
    return 0;
}