CC 		    = gcc
CFLAGS 		= -Wall -Wextra
LDFLAGS 	=

SRC_DIR 	= src

OBJ_DIR		= build

SRCS 		= $(shell find $(SRC_DIR) -name '*.c')

OBJS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

TARGET 		= a.out

# build rule
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# compilation rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean print

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

print:
    @echo "srcs: $(SRCS)"
    @echo "objs: $(OBJS)"
