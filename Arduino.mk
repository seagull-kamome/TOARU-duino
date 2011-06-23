

AVR_TOOLS_PATH    = /usr/bin
ARDUINO_ETC_PATH  = /etc
AVRDUDE_CONF     = $(ARDUINO_ETC_PATH)/avrdude.conf

ARDUINO_LIBS+=HardwareSerial

CC      = $(AVR_TOOLS_PATH)/avr-gcc
CXX     = $(AVR_TOOLS_PATH)/avr-g++
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR      = $(AVR_TOOLS_PATH)/avr-ar
SIZE    = $(AVR_TOOLS_PATH)/avr-size
NM      = $(AVR_TOOLS_PATH)/avr-nm
REMOVE  = rm -f
MV      = mv -f
CAT     = cat
ECHO    = echo


CPPFLAGS	= -mmcu=$(MCU) -DF_CPU=$(F_CPU) \
	-I. -I$(ARDUINO_DIR) -I$(ARDUINO_DIR)/cores/arduino $(addprefix -I,$(LIB_DIRS)) \
	-g -O99 -Os -w -Wall \
	-ffunction-sections -fdata-sections
CFLAGS        = -std=gnu99
CXXFLAGS      = -fno-exceptions
PDEFLAGS = $(CXXFLAGS) -x c++ -include cores/arduino/pde_main.inc
ASFLAGS       = -mmcu=$(MCU) -I. -x assembler-with-cpp
LDFLAGS       = -mmcu=$(MCU) -lm -Wl,--gc-sections -Os





# Everything gets built in here
OBJDIR  	  = $(MCU)_$(F_CPU)
ifeq ($(OUTPUTDIR),)
OUTPUTDIR=$(OBJDIR)
endif


PDE_SOURCES=$(filter %.pde,$(SOURCES))

SOURCE_DIRS=$(sort $(dir $(PDE_SOURCES)))
#C_SOURCES=$(foreach dir,$(SOURCE_DIRS), $(wildcard $(dir)/*.c))
#CPP_SOURCES=$(foreach dir,$(SOURCE_DIRS), $(wildcard $(dir)/*.cpp))
#ASM_SOURCES=$(foreach dir,$(SOURCE_DIRS), $(wildcard $(dir)/*.S))

ELFS=$(addprefix $(OBJDIR)/,$(PDE_SOURCES:.pde=.elf))
IHEXS=$(addprefix $(OUTPUTDIR)/,$(PDE_SOURCES:.pde=.hex))
EEPROMS=$(addprefix $(OUTPUTDIR)/,$(PDE_SOURCES:.pde=.eep))



CORE_SOURCES=$(wildcard $(ARDUINO_DIR)/cores/arduino/*.cpp) $(wildcard $(ARDUINO_DIR)/cores/arduino/*.c)
CORE_OBJS=$(subst $(ARDUINO_DIR)/,$(OBJDIR)/,$(CORE_SOURCES:.cpp=.o))

LIB_DIRS=$(addprefix $(ARDUINO_DIR)/libraries/,$(ARDUINO_LIBS))
LIB_CXX_SOURCES=$(foreach dir,$(LIB_DIRS),$(wildcard $(dir)/*.cpp))
LIB_C_SOURCES=$(foreach dir,$(LIB_DIRS),$(wildcard $(dir)/*.c))
LIB_OBJS=$(subst $(ARDUINO_DIR)/,$(OBJDIR)/,$(LIB_C_SOURCES:.c=.o)) $(subst $(ARDUINO_DIR)/,$(OBJDIR)/,$(LIB_CXX_SOURCES:.cpp=.o))


DEPFILE   = $(OBJDIR)/depends.mk
DEP_FILES=$(CORE_OBJS:.o=.d) $(LIB_OBJS:.o=.d) $(addprefix $(OBJDIR)/,$(PDE_SOURCES:.pde=.d) $(C_SOURCES:.c=.d) $(CXX_SOURCES:.cpp=.d) $(ASM_SOURCES:.S=.d))

OBJS=$(CORE_OBJS) $(LIB_OBJS) $(addprefix $(OBJDIR)/,$(PDE_SOURCES:.pde=.o) $(C_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o) $(ASM_SOURCES:.S=.o))

all: $(OBJS) $(ELFS) $(IHEXS) $(EEPROMS)
clean:
	$(RM) -rf $(OBJDIR)
	$(RM) $(IHEXS) $(EEPROMS)
	$(RM) -rf $(addprefix $(OUTPUTDIR)/,$(SOURCE_DIRS))

depends: $(OBJDIR)/objdirs $(DEP_FILES)
	cat $(DEP_FILES) >$(DEPFILE)

$(OBJDIR)/objdirs:
	mkdir -p $(addprefix $(OBJDIR)/,$(SOURCE_DIRS))
	mkdir -p $(OBJDIR)/cores/arduino
	mkdir -p $(OBJDIR)/libraries $(addprefix $(OBJDIR)/libraries/,$(ARDUINO_LIBS))
	mkdir -p $(addprefix $(OUTPUTDIR)/,$(SOURCE_DIRS))
	touch $(OBJDIR)/objdirs


upload: $(OUTPUTDIR)/$(TARGET).hex $(OUTPUTDIR)/$(TARGET).eep
	$(AVR_TOOLS_PATH)/avrdude -C $(AVRDUDE_CONF)\
		-p $(MCU) \
		-P $(firstword $(wildcard $(ARDUINO_PORT))) -b $(AVRDUDE_ARD_BAUDRATE) \
		-c $(AVRDUDE_ARD_PROGRAMMER) \
		-U flash:w:$(OUTPUTDIR)/$(TARGET).hex:i \
		-U eeprom:w:$(OUTPUTDIR)/$(TARGET).eep:i


#
# Rules for Core and Libraries
#

$(OBJDIR)/%.o : $(ARDUINO_DIR)/%.c
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
$(OBJDIR)/%.o : $(ARDUINO_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
$(OBJDIR)/%.o : $(ARDUINO_DIR)/%.S
	$(CXX) $(CPPFLAGS) $(ASFLAGS) -c -o $@ $<

$(OBJDIR)/%.o : %.pde
	$(CXX) $(CPPFLAGS) $(PDEFLAGS) -c -o $@ $<
$(OBJDIR)/%.o : %.c
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
$(OBJDIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
$(OBJDIR)/%.o : %.S
	$(CXX) $(CPPFLAGS) $(ASFLAGS) -c -o $@ $<


$(OBJDIR)/%.d : $(ARDUINO_DIR)/%.cpp
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -MF $@ -MT $(@:.d=.o)
$(OBJDIR)/%.d : %.pde
	$(CXX) -MM $(CPPFLAGS) $(PDEFLAGS) $< -MF $@ -MT $(@:.d=.o)
$(OBJDIR)/%.d : %.cpp
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -MF $@ -MT $(@:.d=.o)
$(OBJDIR)/%.d : %.c
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -MF $@ -MT $(@:.d=.o)
$(OBJDIR)/%.d : %.S
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -MF $@ -MT $(@:.d=.o)


#
# Linking
#
$(OBJDIR)/%.elf : $(OBJDIR)/%.o $(CORE_OBJS) $(LIB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $< $(wildcard $(dir $(%).pde)/*.S:.S=.o) \
		$(CORE_OBJS) $(LIB_OBJS)


#
# Objcopy
#
$(OUTPUTDIR)/%.hex: $(OBJDIR)/%.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(OUTPUTDIR)/%.eep: $(OBJDIR)/%.elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
		--change-section-lma .eeprom=0 -O ihex $< $@

$(OBJDIR)/%.lss: $(OBJDIR)/%.elf
	$(OBJDUMP) -h -S $< > $@

$(OBJDIR)/%.sym: $(OBJDIR)/%.elf
	$(NM) -n $< > $@


.PHONY:	all clean depends upload objdirs

-include $(DEPFILE)
