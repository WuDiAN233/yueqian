#定义变量表示目标，变量习惯性大写
TARGET=main
#自动搜索匹配当前路径下任意.c
SRC=$(wildcard *.c)
#定义变量存放编译得到的.o文件
OBJ=$(patsubst %.c, %.o, $(SRC))
#定义变量表示编译器
CC=gcc

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $(TARGET)
	./$(TARGET)
	
#添加伪目标
clean:
	rm *.o $(TARGET)