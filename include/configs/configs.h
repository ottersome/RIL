#pragma once
#include <iostream>
#include <string>
#include <mutex>
#include <map>

namespace RIL{

    class Configs{

        public:

            static Configs * Get(const std::string & config_loc);
            static Configs * Get();

            Configs(Configs &other) = delete;
            void operator=(const Configs &other) = delete;

            std::string & getKernelPath() const;


        private:

            Configs(const std::string read_config_location): 
                read_config_location(read_config_location){
                    std::cout << "Loading Configuration from:" << read_config_location;

                    load_data();
                }
            ~Configs(){
                //TODO: perform proper synchonization here
                // basically we shoudl ensure no one is using this
                std::lock_guard<std::mutex> lock(_mutex);
                free(_instance);
            }
            //Simply load/initializas data
            void load_data();

            std::string _kernelpath;

            //--------------------
            // Funcs 
            //--------------------
            void load_configuration_file(){
                //load somehow
            }
            //--------------------
            // Vars
            //--------------------
            static Configs * _instance;
            static std::mutex _mutex;

            //TODO, implement reading from a configuration file
            const std::string read_config_location;

    };
}
