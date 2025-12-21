#include <gtest/gtest.h>
#include <stdexcept>

#include <cstdlib>
#include <string>

#include "pluginLoader.hpp"

TEST(PluginLoader_, a1)
{
    PluginLoader loader;
    EXPECT_THROW(loader.load("definitely_not_exists_12345.so"), std::runtime_error);
}

TEST(PluginLoader_, a2)
{
    PluginLoader loader;

    EXPECT_THROW(loader.load("libc.so.6"), std::runtime_error);
}


TEST(PluginLoader_, a3) {
    PluginLoader loader;
    EXPECT_THROW(loader.load("definitely_not_exists_12345.so"), std::runtime_error);
}
