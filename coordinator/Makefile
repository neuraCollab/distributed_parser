CXX = g++
CXXFLAGS = -std=c++17 -I../include -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lgrpc++ -lprotobuf -pthread

SRCS = coordinator.cpp main.cpp ../generated/task.pb.cc ../generated/task.grpc.pb.cc
OBJS = $(SRCS:.cpp=.o)

TARGET = coordinator

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
    rm -f $(OBJS) $(TARGET)
