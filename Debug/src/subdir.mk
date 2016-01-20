################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Board.cpp \
../src/Field.cpp \
../src/GameOfLife.cpp \
../src/HelpText.cpp \
../src/RLEParser.cpp \
../src/Snake.cpp 

C_SRCS += \
../src/tinyfiledialogs.c 

OBJS += \
./src/Board.o \
./src/Field.o \
./src/GameOfLife.o \
./src/HelpText.o \
./src/RLEParser.o \
./src/Snake.o \
./src/tinyfiledialogs.o 

CPP_DEPS += \
./src/Board.d \
./src/Field.d \
./src/GameOfLife.d \
./src/HelpText.d \
./src/RLEParser.d \
./src/Snake.d 

C_DEPS += \
./src/tinyfiledialogs.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__ -DUSE_QT -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


