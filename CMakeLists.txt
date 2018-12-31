cmake_minimum_required(VERSION 3.0)
project(kasa)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_BINARY_DIR}/kasa_autogen/include)

find_package(ECM 5.10 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(FeatureSummary)

find_package(PkgConfig)

find_package(LibOfx)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)
find_package(Qt5Charts CONFIG REQUIRED)

set(kasa_SRC
    src/main.cpp
    src/mainwindow.cpp
    src/ofx/import/ofximporter.cpp
    src/datamodels/qaccount.cpp
    src/datamodels/qstatement.cpp
    src/datamodels/qtransaction.cpp
    src/database/db.cpp
    src/database/dbdao.cpp
    src/transactionswindow.cpp
    src/itemmodels/tagsitemmodel.cpp
    src/itemmodels/transactionsitemmodel.cpp
    src/views/transactionitemdelegate.cpp
    src/views/transactioneditor.cpp
    src/accountmanager.cpp
    src/itemmodels/transactionsfilterproxymodel.cpp)

# Tell CMake to create the helloworld executable
add_executable(kasa ${kasa_SRC})

target_include_directories(kasa PRIVATE ${CMAKE_SOURCE_DIR}/src/)

target_link_libraries(kasa Qt5::Widgets
                           Qt5::Sql
                           Qt5::Charts
                           ${LIBOFX_LIBRARIES})

# Install the executable
install(TARGETS kasa DESTINATION bin)
