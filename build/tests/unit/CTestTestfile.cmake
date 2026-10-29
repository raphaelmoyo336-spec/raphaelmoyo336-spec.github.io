# CMake generated Testfile for 
# Source directory: /home/thabiso-raphael/Documents/kogence-ide/tests/unit
# Build directory: /home/thabiso-raphael/Documents/kogence-ide/build/tests/unit
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(unit.editor "/home/thabiso-raphael/Documents/kogence-ide/build/tests/unit/test_editor" "-v1")
set_tests_properties(unit.editor PROPERTIES  ENVIRONMENT "QT_QPA_PLATFORM=offscreen" LABELS "unit" TIMEOUT "30" _BACKTRACE_TRIPLES "/home/thabiso-raphael/Documents/kogence-ide/tests/unit/CMakeLists.txt;18;add_test;/home/thabiso-raphael/Documents/kogence-ide/tests/unit/CMakeLists.txt;0;")
