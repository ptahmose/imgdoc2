#pragma once

#include <cstdint>

namespace imgdoc2
{
    struct VersionInfo
    {
        std::uint32_t major{ 0 };
        std::uint32_t minor{ 0 };;
        std::uint32_t patch{ 0 };;

        /// The compiler identification. This is a free-form string.
        std::string compiler_identification;

        std::string build_type;

        /// The URL of the repository - if available.
        std::string repository_url;

        /// The branch - if available.
        std::string repository_branch;

        /// The tag or hash of the repository - if available.
        std::string repository_tag;
    };
}