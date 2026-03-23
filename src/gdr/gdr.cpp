#include <Geode/Geode.hpp>

#include "gdr.hpp"

cocos2d::CCPoint dataFromString(std::string dataString) {
    std::stringstream ss(dataString);
    std::string item;

    float xPos = 0.f;
    float yPos = 0.f;

    for (int i = 0; i < 3; i++) {
        std::getline(ss, item, ',');
        if (i == 1)
            xPos = geode::utils::numFromString<float>(item).unwrapOr(0.f);
        else if (i == 2)
            yPos = geode::utils::numFromString<float>(item).unwrapOr(0.f);
    }

    return { xPos, yPos };
}

geode::prelude::VersionInfo getVersion(std::vector<std::string> nums) {
    if (nums.size() < 3) return geode::prelude::VersionInfo(0, 0, 0);
    size_t major = geode::utils::numFromString<int>(nums[0]).unwrapOr(0);
    size_t minor = geode::utils::numFromString<int>(nums[1]).unwrapOr(0);
    size_t patch = geode::utils::numFromString<int>(nums[2]).unwrapOr(0);
    return geode::prelude::VersionInfo(major, minor, patch);
}
