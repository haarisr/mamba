# Generate fonts.cpp from font files
file(READ "${FONT_DIR}/Roboto-Regular.ttf" FONT_DATA HEX)
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," ROBOTO_FONT_ARRAY "${FONT_DATA}")
configure_file("${FONT_DIR}/fonts.cpp.in" "${OUTPUT_FILE}" @ONLY)
