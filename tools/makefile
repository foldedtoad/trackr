#------------------------------------------------------------------------------
# Convert trackr.bin file into trackr.hex file
#  1) Generate trackr.hex from trackr.bin 
#  2) Test that tracker.hex is good: 
#     Use new trackr.hex to (re)generate "compare.bin" 
#     Compare original trackr.bin with compare.bin 
#     if no "differ", then good conversion
#
#  NOTE: OFFSET of 0x16000 is for SoftDevice 7.x.x, which is what Trackr uses.
#------------------------------------------------------------------------------

NAME      = trackr
BIN_NAME  = ./$(NAME).bin
HEX_NAME  = ./$(NAME).hex
CMP_NAME  = ./compare.bin
OFFSET    = --change-addresses 0x16000

OBJCOPY   = arm-none-eabi-objcopy
DIFF      = diff

#------------------------------------------------------------------------------
#
#------------------------------------------------------------------------------
all: $(HEX_NAME)

$(HEX_NAME):
	$(OBJCOPY) -Ibinary -Oihex $(OFFSET) $(BIN_NAME) $(HEX_NAME)
	$(OBJCOPY) -Obinary -Iihex $(HEX_NAME) $(CMP_NAME)
	$(DIFF) $(BIN_NAME) $(CMP_NAME)

#------------------------------------------------------------------------------
#
#------------------------------------------------------------------------------
clean:
	$(RM) ./*.hex
	$(RM) ./compare.bin
