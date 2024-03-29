# generate_compiler_options.cmake
# Arguments: -DOUTPUT_FILE=<path to output file> -DCOMPILE_OPTIONS=<compile options as string> -DCOMPILE_DEFINITIONS=<compile definitions as string>

# Parse arguments
set(output_file "")
set(compile_options "")
set(compile_definitions "")
foreach(arg IN LISTS ARGN)
    if("${arg}" MATCHES "^-DOUTPUT_FILE=(.*)$")
        set(output_file "${CMAKE_MATCH_1}")
    elseif("${arg}" MATCHES "^-DCOMPILE_OPTIONS=(.*)$")
        set(compile_options "${CMAKE_MATCH_1}")
    elseif("${arg}" MATCHES "^-DCOMPILE_DEFINITIONS=(.*)$")
        set(compile_definitions "${CMAKE_MATCH_1}")
    endif()
endforeach()

# Format as a C++ string
string(REPLACE ";" "\\n" compile_options_formatted "${compile_options}")
string(REPLACE ";" "\\n-D" compile_definitions_formatted "${compile_definitions}")
set(compiler_options_string "\"${compile_options_formatted}\\n-D${compile_definitions_formatted}\"")

# Write to file
file(WRITE "${output_file}" #ifndef COMPILER_OPTIONS_H\n#define COMPILER_OPTIONS_H\n")
file(APPEND "${output_file}" constexpr char COMPILER_OPTIONS[] = ${compiler_options_string};\n")
file(APPEND "${output_file}" "#endif // COMPILER_OPTIONS_H\n")
