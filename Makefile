# ─────────────────────────────────────────────
#  Configurações do projeto
# ─────────────────────────────────────────────
PROJECT     = simpleGEO
RAYLIB_PATH = C:/raylib/raylib

# ─────────────────────────────────────────────
#  Detecção de sistema operacional
# ─────────────────────────────────────────────
ifeq ($(OS),Windows_NT)
    PLATFORM      = WINDOWS
    COMPILER_PATH = C:/raylib/w64devkit/bin
    export PATH  := $(COMPILER_PATH):$(PATH)
else
    PLATFORM = LINUX
endif

# ─────────────────────────────────────────────
#  Flags de compilação
# ─────────────────────────────────────────────
CC     = g++
CFLAGS = -Wall -std=c++14 -D_DEFAULT_SOURCE -Wno-missing-braces

BUILD_MODE ?= DEBUG
ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -O0
else
    CFLAGS += -s -O1
endif

# ─────────────────────────────────────────────
#  Includes e bibliotecas
# ─────────────────────────────────────────────
ifeq ($(PLATFORM),WINDOWS)
    INCLUDE_PATHS = -I. -I./src -I$(RAYLIB_PATH)/src
    LDFLAGS       = -L$(RAYLIB_PATH)/src
    LDLIBS        = -lraylib -lopengl32 -lgdi32 -lwinmm
else
    INCLUDE_PATHS = -I. -I./src
    LDFLAGS       = 
    LDLIBS        = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# ─────────────────────────────────────────────
#  Arquivos fonte e objetos
# ─────────────────────────────────────────────
SRC_DIR = src
OBJ_DIR = obj

# Busca todos os .cpp recursivamente
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SRC  = $(call rwildcard,$(SRC_DIR)/,*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Cria as subpastas de obj/ antes de compilar
ifeq ($(PLATFORM),WINDOWS)
    $(foreach d,$(sort $(dir $(OBJS))),$(shell if not exist "$(subst /,\,$d)" mkdir "$(subst /,\,$d)"))
else
    $(foreach d,$(sort $(dir $(OBJS))),$(shell mkdir -p $d))
endif

# ─────────────────────────────────────────────
#  Regras
# ─────────────────────────────────────────────
.PHONY: all clean run

all: $(PROJECT).exe

$(PROJECT).exe: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS)

ifeq ($(PLATFORM),WINDOWS)
clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(PROJECT).exe del /q $(PROJECT).exe
else
clean:
	rm -rf $(OBJ_DIR) $(PROJECT).exe
endif

run: all
	./$(PROJECT).exe