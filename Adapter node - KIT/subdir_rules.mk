################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
AdapterNode_main.obj: ../AdapterNode_main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\velapulsar\Libraries\Nwk\velapulsar_nwk_config.dat" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\Velapulsar\Libraries\Velapulsar\velapulsar_app_settings.dat"  -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Nwk" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/RF95" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Mac" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Radio" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=KIT --define=TARGET_IS_MSP432P4XX --define=ccs --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="AdapterNode_main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_msp432p401r_ccs.obj: ../startup_msp432p401r_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\velapulsar\Libraries\Nwk\velapulsar_nwk_config.dat" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\Velapulsar\Libraries\Velapulsar\velapulsar_app_settings.dat"  -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Nwk" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/RF95" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Mac" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Radio" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=KIT --define=TARGET_IS_MSP432P4XX --define=ccs --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="startup_msp432p401r_ccs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

system_msp432p401r.obj: ../system_msp432p401r.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\velapulsar\Libraries\Nwk\velapulsar_nwk_config.dat" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\Velapulsar\Libraries\Velapulsar\velapulsar_app_settings.dat"  -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Nwk" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/RF95" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Mac" --include_path="C:/Users/ghassan/Dropbox/Apps/CCS Cloud/Velapulsar/Libraries/Radio" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=KIT --define=TARGET_IS_MSP432P4XX --define=ccs --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="system_msp432p401r.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


