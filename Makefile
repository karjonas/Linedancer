CXX := g++
CC_FLAGS := -std=c++14 -g -O0
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -L/usr/lib -lallegro -lallegro_primitives -lallegro_font \
 -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_image
INCLUDE := -I./src -I/usr/include/allegro5

all: ld35 

clean:
	rm -rf obj ld35

ld35: $(OBJ_FILES)
	$(CXX) $^ -o $@ $(CC_FLAGS) $(LD_FLAGS)

obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CC_FLAGS) -c -o $@ $<
