# extract_linker_script.cmake

# Read the full linker output
file(READ "${DEFAULT_LD_FULL_PATH}" LD_OUTPUT)

# Use a simple regex to extract the default linker script between '====' lines
string(REGEX REPLACE ".*=+\n(.*)\n=+.*" "\\1" LD_SCRIPT_CONTENTS "${LD_OUTPUT}")

# Write the extracted linker script to default.ld
file(WRITE "${DEFAULT_LD_PATH}" "${LD_SCRIPT_CONTENTS}")