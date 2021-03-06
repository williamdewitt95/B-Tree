
CC = g++



# Setup names for custom directories -- make sure there are no spaces after the directory names
BUILD_DIR   = build


# Setup objects  (add new object files here an create a target line for them below 
OBJS        = album.o createBT.o

BUILD_OBJS  = $(addprefix $(BUILD_DIR)/, $(OBJS))

.PHONY: all
all: build myCreateBT tree.ind

tree.ind:
	touch tree.ind

build:
	mkdir build

myCreateBT: $(BUILD_OBJS) btree.hpp
	$(CC) -o myCreateBT $(BUILD_OBJS)

$(BUILD_DIR)/createBT.o: createBT.cpp btree.hpp
	$(CC) createBT.cpp -c -o $(BUILD_DIR)/createBT.o

# $(BUILD_DIR)/btree.o: btree.hpp
# 	$(CC) btree.hpp -c -o $(BUILD_DIR)/btree.o

# $(BUILD_DIR)/btree.o: btree.h btree.cpp
#	 $(CC) btree.cpp -c -o $(BUILD_DIR)/btree.o

# keytype: keyType.h keyType.cpp
# 	$(CC) keyType.cpp -c -o $(BUILD_DIR)/keyType.o

$(BUILD_DIR)/album.o: album.cpp album.h
	$(CC) album.cpp -c -o $(BUILD_DIR)/album.o

# $(BUILD_DIR)/updateBT.o: $(BUILD_OBJS)


# myUpdateBT:
	


.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f myCreateBT
	rm -f myUpdateBT
	rm -f tree.ind

.PHONY: distclean
distclean: clean
	rm -rf $(BUILD_DIR)

.PHONY: pristine
pristine: distclean
