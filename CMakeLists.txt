cmake_minimum_required(VERSION 3.5)
project(MessageQueueTest)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_definitions(-DAMQP_PROVIDER)
if (NOT AMQP_PROVIDER OR ${AMQP_PROVIDER} STREQUAL RABBIT)
    add_definitions(-DRABBITMQ)
    set(AMQP_PROVIDER_SRC src/rabbitmqclient/rabbitmqclient.cpp src/rabbitmqclient/receivingqueue.cpp
                          src/rabbitmqclient/receivingqueueworker.cpp src/rabbitmqclient/sendingqueue.cpp src/rabbitmqclient/topic.cpp)
    set(AMQP_PROVIDER_HEADERS "include/rabbitmqclient/envelope.h"
                              "include/rabbitmqclient/rabbitmqclient.h" "include/rabbitmqclient/receivingqueue.h"
                              "include/rabbitmqclient/receivingqueueworker.h" "include/rabbitmqclient/sendingqueue.h"
                              "include/rabbitmqclient/topic.h")
    set(AMQP_PROVIDER_LIBS SimpleAmqpClient)
elseif(${AMQP_PROVIDER} STREQUAL ACTIVE)
    add_definitions(-DACTIVEMQ)
    include_directories(SYSTEM /usr/include/activemq-cpp-3.9.4 /usr/include/apr-1)
    set(AMQP_PROVIDER_SRC src/activemqclient/activemqclient.cpp src/activemqclient/topic.cpp src/activemqclient/receivingqueue.cpp src/activemqclient/sendingqueue.cpp)
    set(AMQP_PROVIDER_HEADERS include/activemqclient/activemqclient.h include/activemqclient/topic.h include/activemqclient/receivingqueue.h include/activemqclient/envelope.h include/activemqclient/sendingqueue.h)
    set(AMQP_PROVIDER_LIBS activemq-cpp)
elseif(${AMQP_PROVIDER} STREQUAL KAFKA)
    add_definitions(-DKAFKA)
    set(AMQP_PROVIDER_SRC src/kafkaclient/topic.cpp src/kafkaclient/kafkaclient.cpp src/kafkaclient/receivingqueueworker.cpp src/kafkaclient/topicworker.cpp)
    set(AMQP_PROVIDER_HEADERS include/kafkaclient/kafkaclient.h include/kafkaclient/topic.h include/kafkaclient/envelope.h include/kafkaclient/receivingqueueworker.h include/kafkaclient/topicworker.h)
    set(AMQP_PROVIDER_LIBS rdkafka++)
endif ()

set(SRC src/main.cpp src/program.cpp)

set(HEADERS "" include/program.h include/minicsv.h include/baseenvelope.h)
add_executable(${PROJECT_NAME} ${SRC} ${HEADERS} ${AMQP_PROVIDER_SRC} ${AMQP_PROVIDER_HEADERS})

set(CMAKE_CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} Qt5::Core ${AMQP_PROVIDER_LIBS})