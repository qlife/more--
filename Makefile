C_SRC = err_msg.c
CPP_SRC = main.cpp

C_OBJ = err_msg.o
CPP_OBJ = main.o

OBJS = $(C_OBJ) $(CPP_OBJ)

all:more--

more--:${OBJS}
	$(CXX) $(OBJS) -o more-- 

.PHONY clean:
	rm -f $(OBJS)