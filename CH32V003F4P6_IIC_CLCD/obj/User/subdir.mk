################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v00x_it.c \
../User/clcdi2c.c \
../User/main.c \
../User/system_ch32v00x.c 

OBJS += \
./User/ch32v00x_it.o \
./User/clcdi2c.o \
./User/main.o \
./User/system_ch32v00x.o 

C_DEPS += \
./User/ch32v00x_it.d \
./User/clcdi2c.d \
./User/main.d \
./User/system_ch32v00x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\SourceCode\MRS\CH32V003F4P6_IIC_CLCD\Debug" -I"E:\SourceCode\MRS\CH32V003F4P6_IIC_CLCD\Core" -I"E:\SourceCode\MRS\CH32V003F4P6_IIC_CLCD\User" -I"E:\SourceCode\MRS\CH32V003F4P6_IIC_CLCD\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

