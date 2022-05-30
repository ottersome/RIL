#include "configs/configs.h"
#include <mutex>

using namespace RIL;

Configs * Configs::_instance{nullptr};
std::mutex Configs::_mutex;

Configs *Configs::Get(const std::string &config_loc){
    //TODO, load external configurations using config_loc
    std::lock_guard<std::mutex> lock(_mutex);
    if(_instance != nullptr){
        _instance = new Configs(config_loc);
    }
    return _instance;

}

Configs *Configs::Get(){
    std::lock_guard<std::mutex> lock(_mutex);
    if(_instance == nullptr){
        //Look for the default configuration file
        const std::string default_config = "./config";
        _instance = Get(default_config);
    }
    return _instance;
}
