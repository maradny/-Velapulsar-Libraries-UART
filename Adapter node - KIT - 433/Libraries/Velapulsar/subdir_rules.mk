################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Libraries/Velapulsar/comms_interface.obj: ../Libraries/Velapulsar/comms_interface.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\velapulsar\Libraries\Nwk\velapulsar_nwk_config.dat" --cmd_file="C:\Users\ma7ma\Desktop\velapulsar\Libraries\Mac\velapulsar_mac_config.dat" --cmd_file="C:\Users\ma7ma\Desktop\velapulsar\Libraries\Velapulsar\velapulsar_app_settings.dat"  -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv7/ccs_base/arm/include/" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Nwk" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ma7ma/Desktop/velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/RF95" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Mac" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Radio" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=F_433 --define=PKT_ACK --define=KIT --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Libraries/Velapulsar/comms_interface.d" --obj_directory="Libraries/Velapulsar" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/Velapulsar/sensor_interface.obj: ../Libraries/Velapulsar/sensor_interface.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\velapulsar\Libraries\Nwk\velapulsar_nwk_config.dat" --cmd_file="C:\Users\ma7ma\Desktop\velapulsar\Libraries\Mac\velapulsar_mac_config.dat" --cmd_file="C:\Users\ma7ma\Desktop\velapulsar\Libraries\Velapulsar\velapulsar_app_settings.dat"  -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv7/ccs_base/arm/include/" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Nwk" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ma7ma/Desktop/velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/RF95" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Mac" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Radio" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=F_433 --define=PKT_ACK --define=KIT --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Libraries/Velapulsar/sensor_interface.d" --obj_directory="Libraries/Velapulsar" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/Velapulsar/velapulsar.obj: ../Libraries/Velapulsar/velapulsar.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\velapulsar\Libraries\Nwk\velapulsar_nwk_config.dat" --cmd_file="C:\Users\ma7ma\Desktop\velapulsar\Libraries\Mac\velapulsar_mac_config.dat" --cmd_file="C:\Users\ma7ma\Desktop\velapulsar\Libraries\Velapulsar\velapulsar_app_settings.dat"  -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv7/ccs_base/arm/include/" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Nwk" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ma7ma/Desktop/velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/RF95" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Mac" --include_path="C:/Users/ma7ma/Desktop/velapulsar/Libraries/Radio" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=F_433 --define=PKT_ACK --define=KIT --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Libraries/Velapulsar/velapulsar.d" --obj_directory="Libraries/Velapulsar" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


