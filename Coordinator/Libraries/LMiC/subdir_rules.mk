################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Libraries/LMiC/aes.obj: ../Libraries/LMiC/aes.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/RF95" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Radio" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/LMiC" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Mac" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=CFG_us915 --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Libraries/LMiC/aes.d" --obj_directory="Libraries/LMiC" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/LMiC/hal.obj: ../Libraries/LMiC/hal.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/RF95" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Radio" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/LMiC" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Mac" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=CFG_us915 --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Libraries/LMiC/hal.d" --obj_directory="Libraries/LMiC" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/LMiC/lmic.obj: ../Libraries/LMiC/lmic.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/RF95" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Radio" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/LMiC" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Mac" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=CFG_us915 --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Libraries/LMiC/lmic.d" --obj_directory="Libraries/LMiC" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/LMiC/oslmic.obj: ../Libraries/LMiC/oslmic.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/RF95" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Radio" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/LMiC" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Mac" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=CFG_us915 --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Libraries/LMiC/oslmic.d" --obj_directory="Libraries/LMiC" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/LMiC/radio.obj: ../Libraries/LMiC/radio.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/RF95" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Radio" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/LMiC" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Mac" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=CFG_us915 --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Libraries/LMiC/radio.d" --obj_directory="Libraries/LMiC" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


