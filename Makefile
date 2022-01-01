CC = g++
GDB = gdb

CFLAGS = -g
LDFLAGS = -lm -lncurses -lm

TARGET = potereides
SRC_DIR = src
ODIR = build

OBJFILES = $(notdir $(basename $(shell find $(SRC_DIR) -name *.cpp)))

all: run

run: $(TARGET)
	./$(ODIR)/$(TARGET) $(MAP) $(SCORE)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(addprefix $(ODIR)/, $(addsuffix .o, $^)) -o $(ODIR)/$(TARGET) $(LDFLAGS)

$(OBJFILES): 
	$(CC) $(CFLAGS) -c ./$(SRC_DIR)/$@.cpp -o $(ODIR)/$@.o $(LDFLAGS)

debug: $(TARGET)
	$(GDB) ./$(ODIR)/$(TARGET)

clean:
	rm -f $(ODIR)/*
