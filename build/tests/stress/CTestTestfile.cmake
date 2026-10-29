# CMake generated Testfile for 
# Source directory: /home/thabiso-raphael/Documents/kogence-ide/tests/stress
# Build directory: /home/thabiso-raphael/Documents/kogence-ide/build/tests/stress
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(stress.repeated_operations "/home/thabiso-raphael/Documents/kogence-ide/build/tests/stress/test_repeated_operations" "-v1")
set_tests_properties(stress.repeated_operations PROPERTIES  ENVIRONMENT "QT_QPA_PLATFORM=offscreen" LABELS "stress;regression" TIMEOUT "120" _BACKTRACE_TRIPLES "/home/thabiso-raphael/Documents/kogence-ide/tests/stress/CMakeLists.txt;20;add_test;/home/thabiso-raphael/Documents/kogence-ide/tests/stress/CMakeLists.txt;0;")
