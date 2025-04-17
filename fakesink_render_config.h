#ifndef DS3D_DATARENDER_FAKESINK_CONFIG_H
#define DS3D_DATARENDER_FAKESINK_CONFIG_H

#include <ds3d/common/common.h>
#include <ds3d/common/func_utils.h>

#include <ds3d/common/hpp/yaml_config.hpp>
#include <ds3d/common/idatatype.h>

namespace ds3d { namespace impl { namespace render {

struct Config {
    config::ComponentConfig compConfig;
    std::string inputRadarmapKey;
    std::string inputExtKey;

};

inline ErrCode parseConfig(const std::string& content, const std::string& path, Config& config)
{
    DS3D_ERROR_RETURN(
        config::parseComponentConfig(content, path, config.compConfig),
        "parse meta_prepocess component config failed");
    YAML::Node node = YAML::Load(config.compConfig.configBody);

    // parse config parameters here
    if (node["input_radarmap_key"]) {
        config.inputRadarmapKey = node["input_radarmap_key"].as<std::string>();
    }

    if (node["input_extrinsic_key"]) {
        config.inputExtKey = node["input_extrinsic_key"].as<std::string>();
    }

    

    return ErrCode::kGood;
}

}}}

#endif  // DS3D_DATARENDER_FAKESINK_CONFIG_H