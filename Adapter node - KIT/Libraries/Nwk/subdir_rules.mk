################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Libraries/Nwk/velapulsar_nwk.obj: ../Libraries/Nwk/velapulsar_nwk.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" --cmd_file="C:\Users\ghassan\Dropbox\Apps\CCS Cloud\velapulsar\Libraries\Nwk\velapulsar_nwk_config.dat" --cmd_file="C:\Users\ghassan\iCloudDrive\PhD\TRL\CCS_Workspace\Velapulsar\Libraries\Mac\velapulsar_mac_config.dat" --cmd_file="C:\Users\ghassan\iCloudDrive\PhD\TRL\CCS_Workspace\Velapulsar\Libraries\Velapulsar\velapulsar_app_settings.dat"  -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv7/ccs_base/arm/include/" --include_path="C:/Users/ghassan/iCloudDrive/PhD/TRL/CCS_Workspace/Velapulsar/Libraries/EnergyManager" --include_path="C:/Users/ghassan/iCloudDrive/PhD/TRL/CCS_Workspace/Velapulsar/Libraries/Nwk" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/ghassan/iCloudDrive/PhD/TRL/CCS_Workspace/Velapulsar" --include_path="C:/ti/msp/msp432ware__3.50.00.02/driverlib/driverlib/MSP432P4xx" --include_path="C:/Users/ghassan/iCloudDrive/PhD/TRL/CCS_Workspace/Velapulsar/Libraries/RF95" --include_path="C:/Users/ghassan/iCloudDrive/PhD/TRL/CCS_Workspace/Velapulsar/Libraries/Velapulsar" --include_path="C:/Users/ghassan/iCloudDrive/PhD/TRL/CCS_Workspace/Velapulsar/Libraries/Mac" --include_path="C:/Users/ghassan/iCloudDrive/PhD/TRL/CCS_Workspace/Velapulsar/Libraries/Radio" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include/" --advice:power=all --gcc --define=__MSP432P401R__ --define=NODE --define=PKT_ACK --define=KIT --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Libraries/Nwk/velapulsar_nwk.d" --obj_directory="Libraries/Nwk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


