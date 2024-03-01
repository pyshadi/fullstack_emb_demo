# Compiler Setup
CC=gcc
CXX=arm-none-eabi-g++
CFLAGS=-I../../Core/Inc -I../inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include
CXXFLAGS=-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# Source Files
C_SOURCES=../src/unity.c \
          Tests/test_1.c \
          Tests/test_LM75.c

CPP_SOURCES=Core/Src/LM75.cpp \
            Core/Src/LM75Wrapper.cpp

# Object Files (Place all object files in the current directory)
C_OBJECTS=$(notdir $(C_SOURCES:.c=.o))
CPP_OBJECTS=$(notdir $(CPP_SOURCES:.cpp=.o))

# Test executable
EXEC=run_tests

# Default target
all: $(EXEC)

$(EXEC): $(C_OBJECTS) $(CPP_OBJECTS)
	$(CXX) -o $@ $^

# Rule to compile C source files
%.o: Unity/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: Tests/%.c
	$(CC) $(CFLAGS) -c $< -o $@

unity.o: ../src/unity.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile C++ source files
%.o: ../../Core/Src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(C_OBJECTS) $(CPP_OBJECTS) $(EXEC)
