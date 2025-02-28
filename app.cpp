#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <string>
#include <map>

class CPURegisters {
public:
    CPURegisters() {
        //register creation
        registers = {{"A", {}}, {"B", {}}, {"C", {}}, {"D", {}}, {"E", {}}};
    }

    void add_random_value() {
        //randomly choose a register, using uniform dist 1 - 100
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(1, 100);

        //storing keys with indexes
        std::vector<std::string> register_keys = {"A", "B", "C", "D", "E"};
        std::uniform_int_distribution<> reg_dist(0, 4);

        // Choose register and generate a random value
        std::string chosen_register = register_keys[reg_dist(gen)];
        int value = dist(gen);

        //push to the top
        registers[chosen_register].insert(registers[chosen_register].begin(), value);
    }

    void display_registers() {
        std::cout << "----------------------\n" << std::endl;
    
        //iterate through all the registers
        for (auto& reg : registers) {
            //if the size of the vector exceeds 10, remove the last element
            if (reg.second.size() > 10) {
                reg.second.pop_back();  // Removes the last element
            }
    
            //display the register name and its values
            std::cout << "Register " << reg.first << ": [ ";
            for (int val : reg.second) {
                std::cout << val << " ";
            }
            std::cout << "]  Length: " << reg.second.size() << "\n";
        }
    
        std::cout << "----------------------\n" << std::endl;
    }
    

private:
    std::map<std::string, std::vector<int>> registers;
};

void update_registers(CPURegisters& cpu) {
    while (true) {
        cpu.add_random_value();
        cpu.display_registers();
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        system("clear");
    }
}

int main() {
    CPURegisters cpu;
    std::cout << "Simulating real-time register updates...\n" << std::endl;
    std::thread update_thread(update_registers, std::ref(cpu));

    // Let the program run for a while and update registers in real-time
    std::this_thread::sleep_for(std::chrono::seconds(10));
    update_thread.join();
    
    return 0;
}
