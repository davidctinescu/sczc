CC = gcc

SRC_DIR = src
OUT_DIR = out
LIB_DIR = $(SRC_DIR)/libs

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)

CFLAGS = -I$(LIB_DIR)

TARGET = $(OUT_DIR)/sczc

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(OUT_DIR)
	$(CC) -o $@ $^

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OUT_DIR)