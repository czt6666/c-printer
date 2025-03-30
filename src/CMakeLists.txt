cmake_minimum_required(VERSION 3.10)
project(PrinterApp)

set(CMAKE_CXX_STANDARD 17)

add_executable(printer_app 
    main.cpp 
    PrinterDiscovery.cpp 
    PrinterManager.cpp 
    FileConverter.cpp 
    PrintJobHandler.cpp 
    OSAdapter.cpp
)

if(WIN32)
    target_link_libraries(printer_app winspool)
elseif(UNIX)
    target_link_libraries(printer_app cups)
endif()
