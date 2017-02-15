################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./AdapterNode_main.obj" \
"./startup_msp432p401r_ccs.obj" \
"./system_msp432p401r.obj" \
"./Libraries/Mac/velapulsar_mac.obj" \
"./Libraries/Nwk/velapulsar_nwk.obj" \
"./Libraries/RF95/delay.obj" \
"./Libraries/RF95/rf95.obj" \
"./Libraries/RF95/spi_interface.obj" \
"./Libraries/Velapulsar/comms_interface.obj" \
"./Libraries/Velapulsar/sensor_interface.obj" \
"./Libraries/Velapulsar/velapulsar.obj" \
"../msp432p401m.cmd" \
"C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx/ccs/msp432p4xx_driverlib.lib" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Libraries/Mac/subdir_vars.mk
-include Libraries/Nwk/subdir_vars.mk
-include Libraries/RF95/subdir_vars.mk
-include Libraries/Velapulsar/subdir_vars.mk
-include subdir_rules.mk
-include Libraries/Mac/subdir_rules.mk
-include Libraries/Nwk/subdir_rules.mk
-include Libraries/RF95/subdir_rules.mk
-include Libraries/Velapulsar/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Velapulsar.out \

EXE_OUTPUTS__QUOTED += \
"Velapulsar.out" \

BIN_OUTPUTS += \
Velapulsar.hex \

BIN_OUTPUTS__QUOTED += \
"Velapulsar.hex" \


# All Target
all: Velapulsar.out

# Tool invocations
Velapulsar.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP432 Linker'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\velapulsar\Libraries\Nwk\velapulsar_nwk_config.dat" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\Velapulsar\Libraries\Velapulsar\velapulsar_app_settings.dat"  -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --advice:power=all --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_warning=225 --diag_wrap=off --display_error_number -z -m"Velapulsar.map" --stack_size=512 -i"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/lib" -i"C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries" -i"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --reread_libs --warn_sections --diag_wrap=off --display_error_number --xml_link_info="Velapulsar_linkInfo.xml" --rom_model -o "Velapulsar.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

Velapulsar.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP432 Hex Utility'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armhex"  -o "Velapulsar.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "AdapterNode_main.d" "startup_msp432p401r_ccs.d" "system_msp432p401r.d" "Libraries\Mac\velapulsar_mac.d" "Libraries\Nwk\velapulsar_nwk.d" "Libraries\RF95\delay.d" "Libraries\RF95\rf95.d" "Libraries\RF95\spi_interface.d" "Libraries\Velapulsar\comms_interface.d" "Libraries\Velapulsar\sensor_interface.d" "Libraries\Velapulsar\velapulsar.d" 
	-$(RM) "AdapterNode_main.obj" "startup_msp432p401r_ccs.obj" "system_msp432p401r.obj" "Libraries\Mac\velapulsar_mac.obj" "Libraries\Nwk\velapulsar_nwk.obj" "Libraries\RF95\delay.obj" "Libraries\RF95\rf95.obj" "Libraries\RF95\spi_interface.obj" "Libraries\Velapulsar\comms_interface.obj" "Libraries\Velapulsar\sensor_interface.obj" "Libraries\Velapulsar\velapulsar.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

