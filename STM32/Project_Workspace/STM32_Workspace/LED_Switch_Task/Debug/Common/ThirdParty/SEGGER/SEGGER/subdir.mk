################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.c \
/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.c \
/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o \
./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.o \
./Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.d \
./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.d \
./Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o: /home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.c Common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/FreeRTOS/include -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/config -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/OS -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o: /home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S Common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/config -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.o: /home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.c Common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/FreeRTOS/include -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/config -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/OS -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o: /home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.c Common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/FreeRTOS/include -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/config -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/OS -I/home/dhawal/STM32_RTOS/Project_Workspace/STM32_Workspace/Common/ThirdParty/SEGGER/SEGGER -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-ThirdParty-2f-SEGGER-2f-SEGGER

clean-Common-2f-ThirdParty-2f-SEGGER-2f-SEGGER:
	-$(RM) ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.cyclo ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.d ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.su ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.cyclo ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.d ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.o ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.su ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.cyclo ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.d ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o ./Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.su

.PHONY: clean-Common-2f-ThirdParty-2f-SEGGER-2f-SEGGER

