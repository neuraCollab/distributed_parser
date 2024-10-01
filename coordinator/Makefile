CXX = g++
CXXFLAGS = -std=c++17 -I. -Wall
LDFLAGS = -lgrpc++ -lprotobuf -lpthread -lcurl -lxml2

# Файлы проекта
SRCS = coordinator.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = coordinator

# Правило для сборки всего проекта
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для очистки скомпилированных файлов
clean:
	rm -f $(OBJS) $(TARGET)
